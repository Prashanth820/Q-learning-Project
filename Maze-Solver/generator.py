import pygame
import numpy as np

class Generator:

    # colors
    white = (255, 255, 255)
    black = (0, 0, 0)

    # Setting Height and Width
    display_width, display_height = 500, 500

    # initialising the maze
    rows, cols = 8, 8
    maze = np.ones((rows, cols))

    def get_width(self):
        return self.display_width

    def get_height(self):
        return self.display_height

    def generate(self):

        walls = []
        pygame.init()
        screen = pygame.display.set_mode((self.display_width, self.display_height))
        screen.fill(self.white)

        block_width, block_height = self.display_width / self.cols, self.display_height / self.rows
        quit = False
        while not quit:
            pygame.display.update()  # updating the display
            for event in pygame.event.get():
                if event.type == pygame.MOUSEBUTTONDOWN:
                    x, y = pygame.mouse.get_pos()
                    X, Y = int(x / block_width), int(y / block_height)
                    if event.button == 1:  # if left mouse button clicked, add it to walls
                        if (X, Y) not in walls:
                            walls.append((X, Y))
                            pygame.draw.rect(screen, self.black, [X * block_width, Y * block_height, block_width,
                                                             block_height])  # draw a black rectangle
                    elif event.button == 3:  # if right mouse button clicked, remove it from walls
                        try:
                            walls.remove((X, Y))
                        except:
                            pass
                        pygame.draw.rect(screen, self.white, [X * block_width, Y * block_height, block_width,
                                                         block_height])  # draw a black rectangle

                elif event.type == pygame.QUIT:
                    quit = True
                    pygame.quit()

        for x, y in walls:
            self.maze[y, x] = 0

        # adding boundaries
        maze = np.pad(self.maze, [(1, 1), (1, 1)], mode='constant')

        return maze
