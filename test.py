import math
import matplotlib.pyplot as plt

def circular_movement(t, r=5):
    """
    Calcule la position (x, y) d'un élément qui se déplace en cercle.
    
    :param t: Temps ou paramètre d'avancement.
    :param r: Rayon du cercle.
    :return: Tuple (x, y) représentant la position actuelle.
    """
    theta = t * 0.1  # Angle (varie avec le temps, ajuster 0.1 pour la vitesse angulaire)
    x = r * math.cos(theta)
    y = r * math.sin(theta)
    return x, y

# Exemple d'animation simple
positions = []
for t in range(100):  # 100 étapes de temps
    positions.append(circular_movement(t))

# Tracer les positions
x_vals, y_vals = zip(*positions)
plt.plot(x_vals, y_vals, label="Trajectoire circulaire")
plt.scatter(x_vals[-1], y_vals[-1], color="red", label="Position finale")  # Dernier point
plt.title("Déplacement en cercle")
plt.xlabel("x")
plt.ylabel("y")
plt.legend()
plt.grid(True)
plt.axis('equal')  # Cercle bien proportionné
plt.show()
