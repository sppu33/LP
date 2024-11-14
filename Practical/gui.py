import tkinter as tk
from tkinter import messagebox

class Railways:
    def __init__(self, root):
        self.root = root
        self.root.geometry("400x400")
        self.root.title("Railway Management System")
        self.root.config(bg="grey")

        tk.Label(root, text="Railway Management", font=("Arial", 24), bg="grey", fg="blue").pack(pady=10)
        self.tickets = []

        tk.Label(root, text="Passenger Name: ",bg="grey").pack()
        self.name_entry = tk.Entry(root)
        self.name_entry.pack(pady=5)

        tk.Label(root, text="Train No: ",bg="grey").pack()
        self.train_entry = tk.Entry(root)
        self.train_entry.pack(pady=5)

        tk.Button(root, text="Book Tickets", command=self.booking, bg="green", fg="white", width=20, height=2).pack(pady=7)
        tk.Button(root, text="Cancel Ticket", command=self.canceling, fg="white", bg="red", width=20, height=2).pack(pady=7)
        tk.Button(root, text="Display Ticket", command=self.display, fg="white", bg="blue", width=20, height=2).pack(pady=7)

    def booking(self):
        name = self.name_entry.get()
        train = self.train_entry.get()
        if name and train:
            self.tickets.append((name, train))
            messagebox.showinfo("Success", "Tickets Booked")
        else:
            messagebox.showwarning("Input Error", "Enter correct fields")

    def canceling(self):
        name = self.name_entry.get()
        train = self.train_entry.get()
        if (name, train) in self.tickets:
            self.tickets.remove((name, train))
            messagebox.showinfo("Success", "Tickets are cancelled")
        else:
            messagebox.showwarning("Invalid Info", "Enter correct info")

    def display(self):
        if self.tickets:
            ticket_list = "\n".join([f"{name} - Train {train}" for name, train in self.tickets])
            messagebox.showinfo("Bookings Are:", ticket_list)
        else:
            messagebox.showwarning("Error", "No Bookings Available")

root = tk.Tk()
app = Railways(root)
root.mainloop()
