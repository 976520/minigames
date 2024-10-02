import pygame, sys, random

pygame.init()
game_screen = pygame.display.set_mode((300, 300))
pygame.display.set_caption('asfd')
game_screen.fill((28, 170, 156))

game_board = [[0]*3 for _ in range(3)]

def draw_figures():
    for row in range(3):
        for column in range(3):
            if game_board[row][column] == 1:
                pygame.draw.circle(game_screen, (239, 231, 200), (column * 100 + 50, row * 100 + 50), 33, 15)
            elif game_board[row][column] == 2:
                pygame.draw.line(game_screen, (84, 84, 84), (column * 100 + 25, row * 100 + 75), (column * 100 + 75, row * 100 + 25), 25)
                pygame.draw.line(game_screen, (84, 84, 84), (column * 100 + 25, row * 100 + 25), (column * 100 + 75, row * 100 + 75), 25)

def mark_square(row, column, player): game_board[row][column] = player

def available_square(row, column): return game_board[row][column] == 0

def is_board_full(): return all(game_board[row][column] for row in range(3) for column in range(3))

def check_win(player):
    return any(all(game_board[row][column] == player for column in range(3)) for row in range(3)) or \
           any(all(game_board[row][column] == player for row in range(3)) for column in range(3)) or \
           all(game_board[i][i] == player for i in range(3)) or \
           all(game_board[i][2 - i] == player for i in range(3))

def computer_move():
    available_moves = [(row, column) for row in range(3) for column in range(3) if available_square(row, column)]
    if available_moves:
        move = random.choice(available_moves)
        mark_square(move[0], move[1], 2)
        return check_win(2)
    return False

def display_winner(player): 
    game_screen.blit(pygame.font.Font(None, 74).render('human wins' if player == 1 else 'Computer wins', True, (255, 255, 255)), pygame.font.Font(None, 74).render('human wins' if player == 1 else 'Computer wins', True, (255, 255, 255)).get_rect(center=(150, 150)))
    pygame.display.update()

current_player, game_over = 1, False

for i in range(1, 3):
    pygame.draw.line(game_screen, (23, 145, 135), (0, i * 100), (300, i * 100), 15)
    pygame.draw.line(game_screen, (23, 145, 135), (i * 100, 0), (i * 100, 300), 15)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        if event.type == pygame.MOUSEBUTTONDOWN and not game_over and current_player == 1 and available_square(event.pos[1] // 100, event.pos[0] // 100):
            mark_square(event.pos[1] // 100, event.pos[0] // 100, current_player)
            game_over = check_win(current_player)
            if game_over:
                display_winner(current_player)
            current_player = 2
            draw_figures()
    if current_player == 2 and not game_over:
        game_over = computer_move()
        if game_over:
            display_winner(2)
        current_player = 1
        draw_figures()
    pygame.display.update()
