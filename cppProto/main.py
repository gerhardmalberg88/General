import http.server
import socketserver
import os
import justpy as jp
from tkinter import *

PORT = 8000

Handler = http.server.SimpleHTTPRequestHandler

print("Python is scripting language which is not compiled thereby you can run pythond stright using command \"python3 main.py\"")
print("You can also run other process if you want for example you can run main.cpp which oyu just created ")
os.system("./main")
print("\n\n")
print("or you can create quick GUI")

window=Tk()
btn=Button(window, text="This is Button widget", fg='blue')
btn.place(x=80, y=100)
lbl=Label(window, text="This is Label widget", fg='red', font=("Helvetica", 16))
lbl.place(x=60, y=50)
txtfld=Entry(window, text="This is Entry Widget", bd=5)
txtfld.place(x=80, y=150)
window.title('Hello Python')
window.geometry("300x200+10+10")
window.mainloop()


print("Or you can run server in a following way ")
with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print("serving at port", PORT)
    httpd.serve_forever()



