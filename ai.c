#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include "ai.h"
#include "board.h"

#define MAX_PENGUINS_PER_PLAYER 3
#define MAX_MOVES (MAX_PENGUINS_PER_PLAYER * 4)
#define MINIMAX_DEPTH 3
#define INF_SCORE 1000000000

typedef struct {
    CoordXY to;
    int score;
} PlacementCandidate;

typedef struct {
    int penguinId;
    CoordXY from;
    CoordXY to;
    int score;
} MoveCandidate;

typedef struct {
    int penguinId;
    CoordXY from;
    int fromOldValue;
    CoordXY to;
    int toOldValue;
    int oldScore;
    int oldCanMove;
} MoveUndo;

static int in_bounds(const struct GameState* gameState, int r, int c)
{
    return r >= 0 && c >= 0 && r < gameState->board.rows && c < gameState->board.cols;
}

static int is_walkable_tile(int tile)
{
    return tile >= MIN_FISH && tile <= MAX_FISH;
}

static int tile_for_player(int playerId)
{
    return playerId == 1 ? TILE_P1 : TILE_P2;
}

static int count_walkable_neighbors(
    const struct GameState* gameState,
    int row,
    int col,
    CoordXY blockedCell)
{
    const int dr[4] = { 1, -1, 0, 0 };
    const int dc[4] = { 0, 0, 1, -1 };
    int count = 0;

    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        if (!in_bounds(gameState, nr, nc)) {
            continue;
        }
        if (nr == blockedCell.X && nc == blockedCell.Y) {
            continue;
        }
        if (is_walkable_tile(gameState->board.arr[nr][nc])) {
            ++count;
        }
    }

    return count;
}

static int generate_legal_moves(
    const struct players* player,
    const struct GameState* gameState,
    int playerId,
    MoveCandidate* outMoves,
    int maxMoves)
{
    const int dr[4] = { 1, -1, 0, 0 };
    const int dc[4] = { 0, 0, 1, -1 };
    int playerIndex = playerId - 1;
    int count = 0;

    if (!player || !gameState || !gameState->board.arr || !outMoves || maxMoves <= 0) {
        return 0;
    }
    if (playerIndex < 0 || playerIndex > 1) {
        return 0;
    }

    int placedPenguins = player->playr[playerIndex].numberOfPlacedPenguins;
    if (placedPenguins > MAX_PENGUINS_PER_PLAYER) {
        placedPenguins = MAX_PENGUINS_PER_PLAYER;
    }

    for (int penId = 0; penId < placedPenguins; ++penId) {
        CoordXY from = player->playr[playerIndex].penguins[penId].penguinCoords;

        if (!in_bounds(gameState, from.X, from.Y)) {
            continue;
        }
        if (gameState->board.arr[from.X][from.Y] != tile_for_player(playerId)) {
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = from.X + dr[i];
            int nc = from.Y + dc[i];
            if (!in_bounds(gameState, nr, nc)) {
                continue;
            }

            int tile = gameState->board.arr[nr][nc];
            if (!is_walkable_tile(tile)) {
                continue;
            }
            if (count >= maxMoves) {
                return count;
            }

            outMoves[count].penguinId = penId;
            outMoves[count].from = from;
            outMoves[count].to.X = nr;
            outMoves[count].to.Y = nc;
            outMoves[count].score = tile * 10 + count_walkable_neighbors(gameState, nr, nc, from) * 3;
            ++count;
        }
    }

    return count;
}

static int count_player_mobility(
    const struct players* player,
    const struct GameState* gameState,
    int playerId)
{
    MoveCandidate moves[MAX_MOVES];
    return generate_legal_moves(player, gameState, playerId, moves, MAX_MOVES);
}

static int evaluate_state(
    const struct players* player,
    const struct GameState* gameState,
    int maximizingPlayerId)
{
    int minimizingPlayerId = maximizingPlayerId == 1 ? 2 : 1;
    int maxIndex = maximizingPlayerId - 1;
    int minIndex = minimizingPlayerId - 1;

    int scoreDiff = player->playr[maxIndex].score - player->playr[minIndex].score;
    int mobilityDiff = count_player_mobility(player, gameState, maximizingPlayerId)
        - count_player_mobility(player, gameState, minimizingPlayerId);

    return scoreDiff * 100 + mobilityDiff * 8;
}

static void apply_move_sim(
    struct players* player,
    struct GameState* gameState,
    int playerId,
    const MoveCandidate* move,
    MoveUndo* undo)
{
    int playerIndex = playerId - 1;
    struct Penguin* penguin = &player->playr[playerIndex].penguins[move->penguinId];

    undo->penguinId = move->penguinId;
    undo->from = move->from;
    undo->to = move->to;
    undo->fromOldValue = gameState->board.arr[move->from.X][move->from.Y];
    undo->toOldValue = gameState->board.arr[move->to.X][move->to.Y];
    undo->oldScore = player->playr[playerIndex].score;
    undo->oldCanMove = penguin->can_make_any_move;

    player->playr[playerIndex].score += undo->toOldValue;
    gameState->board.arr[move->from.X][move->from.Y] = TILE_HOLE;
    gameState->board.arr[move->to.X][move->to.Y] = tile_for_player(playerId);
    penguin->penguinCoords = move->to;
    penguin->can_make_any_move = 1;
}

static void undo_move_sim(
    struct players* player,
    struct GameState* gameState,
    int playerId,
    const MoveUndo* undo)
{
    int playerIndex = playerId - 1;
    struct Penguin* penguin = &player->playr[playerIndex].penguins[undo->penguinId];

    gameState->board.arr[undo->from.X][undo->from.Y] = undo->fromOldValue;
    gameState->board.arr[undo->to.X][undo->to.Y] = undo->toOldValue;
    player->playr[playerIndex].score = undo->oldScore;
    penguin->penguinCoords = undo->from;
    penguin->can_make_any_move = undo->oldCanMove;
}

static int minimax(
    struct players* player,
    struct GameState* gameState,
    int currentPlayerId,
    int maximizingPlayerId,
    int depth,
    int alpha,
    int beta)
{
    int otherPlayerId = currentPlayerId == 1 ? 2 : 1;
    MoveCandidate moves[MAX_MOVES];
    MoveCandidate otherMoves[MAX_MOVES];
    int moveCount = generate_legal_moves(player, gameState, currentPlayerId, moves, MAX_MOVES);
    int otherMoveCount = generate_legal_moves(player, gameState, otherPlayerId, otherMoves, MAX_MOVES);

    if (depth == 0 || (moveCount == 0 && otherMoveCount == 0)) {
        return evaluate_state(player, gameState, maximizingPlayerId);
    }

    if (moveCount == 0) {
        return minimax(player, gameState, otherPlayerId, maximizingPlayerId, depth - 1, alpha, beta);
    }

    if (currentPlayerId == maximizingPlayerId) {
        int best = -INF_SCORE;
        for (int i = 0; i < moveCount; ++i) {
            MoveUndo undo;
            apply_move_sim(player, gameState, currentPlayerId, &moves[i], &undo);
            int score = minimax(player, gameState, otherPlayerId, maximizingPlayerId, depth - 1, alpha, beta);
            undo_move_sim(player, gameState, currentPlayerId, &undo);

            if (score > best) {
                best = score;
            }
            if (best > alpha) {
                alpha = best;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return best;
    }
    else {
        int best = INF_SCORE;
        for (int i = 0; i < moveCount; ++i) {
            MoveUndo undo;
            apply_move_sim(player, gameState, currentPlayerId, &moves[i], &undo);
            int score = minimax(player, gameState, otherPlayerId, maximizingPlayerId, depth - 1, alpha, beta);
            undo_move_sim(player, gameState, currentPlayerId, &undo);

            if (score < best) {
                best = score;
            }
            if (best < beta) {
                beta = best;
            }
            if (beta <= alpha) {
                break;
            }
        }
        return best;
    }
}

static int copy_board(const Board* src, Board* outCopy)
{
    if (!src || !outCopy || !src->arr || src->rows <= 0 || src->cols <= 0) {
        return 0;
    }

    *outCopy = create_board(src->rows, src->cols);
    if (!outCopy->arr) {
        return 0;
    }

    for (int r = 0; r < src->rows; ++r) {
        for (int c = 0; c < src->cols; ++c) {
            outCopy->arr[r][c] = src->arr[r][c];
        }
    }
    return 1;
}

int ai_choose_placement(
    const struct GameState* gameState,
    int playerId,
    AIDifficulty difficulty,
    CoordXY* outPlacement)
{
    (void)playerId;

    if (!gameState || !gameState->board.arr || !outPlacement) {
        return 0;
    }

    int capacity = gameState->board.rows * gameState->board.cols;
    if (capacity <= 0) {
        return 0;
    }

    PlacementCandidate* candidates =
        (PlacementCandidate*)malloc((size_t)capacity * sizeof(PlacementCandidate));
    if (!candidates) {
        return 0;
    }

    int count = 0;
    for (int r = 0; r < gameState->board.rows; ++r) {
        for (int c = 0; c < gameState->board.cols; ++c) {
            int tile = gameState->board.arr[r][c];
            int legal = gameState->placingPenguinsOnlyOnOne
                ? (tile == MIN_FISH)
                : is_walkable_tile(tile);

            if (!legal) {
                continue;
            }

            candidates[count].to.X = r;
            candidates[count].to.Y = c;

            int baseScore = tile * 10 + count_walkable_neighbors(gameState, r, c, (CoordXY) { -1, -1 }) * 4;
            if (difficulty == AI_HARD) {
                baseScore += count_walkable_neighbors(gameState, r, c, (CoordXY) { -1, -1 }) * 3;
            }
            candidates[count].score = baseScore;
            ++count;
        }
    }

    if (count == 0) {
        free(candidates);
        return 0;
    }

    if (difficulty == AI_EASY) {
        int idx = rand() % count;
        *outPlacement = candidates[idx].to;
    }
    else if (difficulty == AI_MEDIUM) {
        int best = 0;
        for (int i = 1; i < count; ++i) {
            if (candidates[i].score > candidates[best].score) {
                best = i;
            }
        }
        *outPlacement = candidates[best].to;
    }
    else {
        // Hard placement: one-ply minimax over placement heuristics.
        int bestIndex = 0;
        int bestValue = -INF_SCORE;

        for (int i = 0; i < count; ++i) {
            int opponentBest = -INF_SCORE;
            for (int j = 0; j < count; ++j) {
                if (j == i) {
                    continue; // tile occupied by our hypothetical placement
                }
                if (candidates[j].score > opponentBest) {
                    opponentBest = candidates[j].score;
                }
            }

            int minimaxValue = candidates[i].score - (opponentBest == -INF_SCORE ? 0 : opponentBest);
            if (minimaxValue > bestValue) {
                bestValue = minimaxValue;
                bestIndex = i;
            }
        }

        *outPlacement = candidates[bestIndex].to;
    }

    free(candidates);
    return 1;
}

int ai_choose_move(
    const struct players* player,
    const struct GameState* gameState,
    int playerId,
    AIDifficulty difficulty,
    int* outPenguinId,
    CoordXY* outDestination)
{
    MoveCandidate candidates[MAX_MOVES];
    int count;
    int opponentId = playerId == 1 ? 2 : 1;

    if (!player || !gameState || !gameState->board.arr || !outPenguinId || !outDestination) {
        return 0;
    }

    count = generate_legal_moves(player, gameState, playerId, candidates, MAX_MOVES);
    if (count == 0) {
        return 0;
    }

    if (difficulty == AI_EASY) {
        int idx = rand() % count;
        *outPenguinId = candidates[idx].penguinId;
        *outDestination = candidates[idx].to;
        return 1;
    }

    if (difficulty == AI_MEDIUM) {
        int best = 0;
        for (int i = 1; i < count; ++i) {
            if (candidates[i].score > candidates[best].score) {
                best = i;
            }
        }
        *outPenguinId = candidates[best].penguinId;
        *outDestination = candidates[best].to;
        return 1;
    }

    // Hard mode: shallow minimax with alpha-beta pruning.
    struct players simPlayers = *player;
    struct GameState simState = *gameState;
    simState.board.arr = NULL;
    simState.board.rows = 0;
    simState.board.cols = 0;

    if (!copy_board(&gameState->board, &simState.board)) {
        // Fallback to greedy if memory is tight.
        int best = 0;
        for (int i = 1; i < count; ++i) {
            if (candidates[i].score > candidates[best].score) {
                best = i;
            }
        }
        *outPenguinId = candidates[best].penguinId;
        *outDestination = candidates[best].to;
        return 1;
    }

    int bestScore = -INF_SCORE;
    int bestIndex = 0;
    for (int i = 0; i < count; ++i) {
        MoveUndo undo;
        apply_move_sim(&simPlayers, &simState, playerId, &candidates[i], &undo);
        int score = minimax(
            &simPlayers,
            &simState,
            opponentId,
            playerId,
            MINIMAX_DEPTH - 1,
            -INF_SCORE,
            INF_SCORE);
        undo_move_sim(&simPlayers, &simState, playerId, &undo);

        if (score > bestScore || (score == bestScore && candidates[i].score > candidates[bestIndex].score)) {
            bestScore = score;
            bestIndex = i;
        }
    }

    board_destroy(&simState.board);
    *outPenguinId = candidates[bestIndex].penguinId;
    *outDestination = candidates[bestIndex].to;
    return 1;
}
