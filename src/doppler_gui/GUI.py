import tkinter as tk
import mido
from mido import Message, open_output

# Ouvrir le port MIDI pour Teensy
midi_ports = mido.get_output_names()
print("Ports MIDI disponibles:", midi_ports)
midi_out = open_output(midi_ports[1])  # Vérifier que c'est bien le Teensy

# Variable globale pour l'état de l'effet
effect_on = False  

# Fonction pour envoyer des messages MIDI
def send_midi(cc, value):
    midi_out.send(Message('control_change', control=cc, value=value))
    print(f"MIDI envoyé: CC {cc} -> {value}")

# Fonction pour mettre à jour l'affichage numérique
def update_label(value, label, cc):
    if effect_on:
        midi_value = int((float(value) + 20) * (127 / 40))  # Conversion -20..20 en 0..127
        send_midi(cc, midi_value)
    else:
        doppler_L.set(0)
        doppler_R.set(0)

# Fonction pour activer/désactiver l'effet
def toggle_effect():
    global effect_on  
    effect_on = not effect_on
    send_midi(12, 127 if effect_on else 0)
    effect_label.config(text="Effet: Activé" if effect_on else "Effet: Désactivé", 
                        fg="green" if effect_on else "red")
    
        

# Création de l'interface Tkinter
root = tk.Tk()
root.title("Contrôle MIDI Doppler")
root.geometry("350x250")

# Curseur + affichage numérique pour le Doppler gauche (CC 10)
tk.Label(root, text="Doppler Gauche").pack()
doppler_L = tk.Scale(root, from_=-20, to=20, resolution=1, orient='horizontal',
                     command=lambda v: update_label(v, label_L, 10))
doppler_L.pack()
label_L = tk.Label(root, text="Valeur: 0")
label_L.pack()

# Curseur + affichage numérique pour le Doppler droit (CC 11)
tk.Label(root, text="Doppler Droit").pack()
doppler_R = tk.Scale(root, from_=-20, to=20, resolution=1, orient='horizontal',
                     command=lambda v: update_label(v, label_R, 11))
doppler_R.pack()
label_R = tk.Label(root, text="Valeur: 0")
label_R.pack()

# Indicateur d'état pour l'effet
effect_label = tk.Label(root, text="Effet: Désactivé", fg="red")
effect_label.pack()

# Bouton pour activer/désactiver l'effet (MIDI CC 12)
effect_button = tk.Button(root, text="Activer/Désactiver Effet", command=toggle_effect)
effect_button.pack()

# Lancer Tkinter
root.mainloop()
