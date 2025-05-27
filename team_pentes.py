import subprocess

def solve_maze(labirinto: str) -> float:
    # Executa o programa C e captura a saída
    subprocess.run(['gcc', "main.c", '-o', "maze_solver"], check=True)
    resultado = subprocess.run(['./maze_solver', labirinto], stdout=subprocess.PIPE, text=True)

    # Pega a saída e converte para inteiro
    return float(resultado.stdout.strip())

if __name__ == "__main__":
    labirinto = '''#################
                    #S    #   #     #
                    ##### ### # ### #
                    #   #   # # # # #
                    # # ### # # # # #
                    # # # # #     # #
                    # # # # # ##### #
                    # #   # # # #   #
                    # ##### # # # ###
                    #       #   #   #
                    # ######### ### #
                    # #           # #
                    # ############# #
                    #             # #
                    ############# # #
                    #              E#
                    #################'''
    print(f"Número capturado: {solve_maze(labirinto)}")
