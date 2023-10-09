import matplotlib.pyplot as plt


with open("wisard_field.txt", "r") as file:
    # Créez des listes vides pour chaque colonne
    colonne1 = []
    colonne2 = []
    colonne3 = []
    colonne4 = []

    # Parcourez chaque ligne du fichier
    for ligne in file:
        # Divisez la ligne en valeurs en utilisant l'espace comme séparateur
        valeurs = ligne.strip().split()

        # Assurez-vous qu'il y a au moins 4 valeurs dans la ligne
        if len(valeurs) >= 4 and "h:x" not in valeurs:
            # Ajoutez chaque valeur à la liste appropriée
            colonne1.append(float(valeurs[0])*100)
            colonne2.append(float(valeurs[1])/6*4)
            colonne3.append(float(valeurs[2]))
            colonne4.append(float(valeurs[3]))

            if float(valeurs[1]) == 3.:
                print(float(valeurs[0]))



plt.xlabel("Middle Plasic Position (cm)")
plt.ylabel("B (T)")
plt.scatter(7.5, 4, color = "black", label = "Actual Position")
plt.scatter(40.7, 1, color = "red", label = r"1T - $\Delta = 33.2 cm$")
plt.scatter(35.7, 2, color = "orange", label = r"2T - $\Delta = 28.2 cm$")
plt.scatter(31, 3, color = "green", label = r"3T - $\Delta = 23.5 cm$")
plt.plot(colonne1, colonne2, color='grey', zorder=0)
plt.legend(loc="upper left")
plt.show()