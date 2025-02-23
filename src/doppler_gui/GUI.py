import tkinter as tk
import serial
import time

# Configuration du port série (à adapter selon ton Arduino)
SERIAL_PORT = 'COM8'  # Remplace par le bon port (ex: '/dev/ttyUSB0' sur Linux)
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)  # Attendre que la connexion s'établisse
except Exception as e:
    print(f"Erreur de connexion au port série: {e}")
    ser = None

def send_data():
    if ser:
        factorL = doppler_scale_L.get()
        factorR = doppler_scale_R.get()
        delay = delay_scale.get()
        command = f"{factorL:.5f},{factorR:.5f},{delay}\n"
        ser.write(command.encode('utf-8'))
        print(f"Envoyé: {command}")

def toggle_effect():
    if ser:
        ser.write(b'TOGGLE\n')  # Commande pour activer/désactiver
        print("Effet activé/désactivé")

# Création de l'interface
root = tk.Tk()
root.title("Contrôle Doppler Stéréo")
root.geometry("350x250")

# Curseur pour le facteur Doppler gauche
doppler_label_L = tk.Label(root, text="Facteur Doppler (Gauche)")
doppler_label_L.pack()
doppler_scale_L = tk.Scale(root, from_=-0.02, to=0.02, resolution=0.0001, orient='horizontal', command=lambda x: send_data())
doppler_scale_L.pack()

# Curseur pour le facteur Doppler droit
doppler_label_R = tk.Label(root, text="Facteur Doppler (Droite)")
doppler_label_R.pack()
doppler_scale_R = tk.Scale(root, from_=-0.02, to=0.02, resolution=0.0001, orient='horizontal', command=lambda x: send_data())
doppler_scale_R.pack()

# Curseur pour le délai
delay_label = tk.Label(root, text="Délai")
delay_label.pack()
delay_scale = tk.Scale(root, from_=0, to=1000, resolution=1, orient='horizontal', command=lambda x: send_data())
delay_scale.pack()

# Bouton pour activer/désactiver
button = tk.Button(root, text="Activer/Désactiver", command=toggle_effect)
button.pack()

root.mainloop()

# Fermer la connexion série à la fin
if ser:
    ser.close()
