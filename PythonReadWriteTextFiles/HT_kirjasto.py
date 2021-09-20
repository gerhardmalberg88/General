import datetime, sys                #tuodaan kirjastot

class luku:                         #luokat
    paivat = ""
    arvot = ""
    
class tallentaa:
    maksimi = ""
    minimi = ""
    minpaiva= ""
    maxpaiva = ""
    ka = ""
    kokonaisuus = 0
    
def valikko():                      #valikko aliohjelma
    print("Mitä haluat tehdä:\n"
          "1) Lue kuormatiedot\n"
          "2) Analysoi kuormatiedot\n"
          "3) Tallenna kuormien tulostiedot\n"
          "0) Lopeta")

    valinta = int(input("Valintasi: "))

    return valinta

def lukeminen(listaluku,x):
    listaluku.clear()               #tyhjennetään lista
    rivienmaara = 0
    
    try:                            #virheenkäsittely
            
        tiedosto = open(x,"r")
        tiedosto.readline()         #luetaan otsikkorivi pois

        while True:

            rivi = tiedosto.readline()
            print("Debug1")
            if len(rivi) == 0:
                print("Tiedosto '"+x+"' luettu,",rivienmaara,"riviä.\n")
                tiedosto.close()
                break
            print("Debug2")
            rivi = rivi.split(";")          #jaetaan rivi osiin
        
            rivienmaara += 1
            print("Debug3")
            maarat = luku()
            print("Debug4")
            maarat.paivat = datetime.datetime.strptime(rivi[0]+rivi[1],"%d.%m.%Y%H:%M:%S")
            print("Debug5")
            maarat.arvot = float(rivi[2])
            listaluku.append(maarat)                        #tallenetaan tiedot listaan
            print("Debug4")


    except Exception:                                       #virheenkäsittely
        print("Tiedoston '"+x+"' käsittelyssä virhe, lopetetaan.")
        sys.exit(0)                                         #jos ei toimi lopetetaan
        
    return listaluku


def analysoidaan(listaluku,listatallennus):

    listatallennus.clear()                              #tyhjennetään lista

    try:                                                #virheenkäsittely
        
        ekapaiva = listaluku[0].paivat                  #listan ensimmäinen päivä
        vikapaiva = listaluku[0].paivat
        rivit = 0

        tallennus = tallentaa()
    
        tallennus.maksimi = listaluku[0].arvot          #ensimmäinen arvo
        tallennus.minimi = listaluku[0].arvot

    
        
        for maarat in listaluku:                        #luetaan listaa
            
            rivit += 1
            tallennus.kokonaisuus += maarat.arvot       #kaikki arvot

            if tallennus.maksimi < maarat.arvot:        #etsitään suurin
                tallennus.maksimi = maarat.arvot
                tallennus.maxpaiva = maarat.paivat

            if tallennus.minimi > maarat.arvot:
                tallennus.minimi = maarat.arvot
                tallennus.minpaiva = maarat.paivat

            if ekapaiva > maarat.paivat:                #analysointijakson päivät
                ekapaiva = maarat.paivat

            if vikapaiva < maarat.paivat:
                vikapaiva = maarat.paivat

        tallennus.ka = tallennus.kokonaisuus/rivit
        listatallennus.append(tallennus)

        print("Data analysoitu ajalta",datetime.datetime.strftime(ekapaiva,"%d.%m.%Y"),"-",str(datetime.datetime.strftime(vikapaiva,"%d.%m.%Y"))+".\n")


    except Exception:                                   #virheenkäsittely
        print("Lista on tyhjä. Lue ensin tiedosto.\n")
        pass

    return listatallennus

def tallentaminen(tiedostonnimi1,listatallennus):

    try:                                                #virheenkäsittely
            
        tiedosto = open(tiedostonnimi1,"w")

        for tallennus in listatallennus:                #luetaan tallennuslista

            ero = tallennus.maxpaiva - tallennus.minpaiva       #lasketaan suurimman ja pienimmän erotus

            print("Pienin jätekuorma tuli "+str(datetime.datetime.strftime(tallennus.minpaiva,"%d.%m.%Y"))+" ja oli "+str(int(tallennus.minimi))+" kg.\n"
                  "Suurin jätekuorma tuli "+str(datetime.datetime.strftime(tallennus.maxpaiva,"%d.%m.%Y"))+" ja oli "+str(int(tallennus.maksimi))+" kg.\n"
                  "Pienimmän ja suurimman kuorman toimitusten välissä oli "+str(ero.days)+" päivää.\n"
                  "Analyysijaksolla jätettä tuli yhteensä "+str(int(tallennus.kokonaisuus))+" kg.\n"
                  "Keskimäärin jätekuorma oli "+str(int(tallennus.ka))+" kg.\n"
                  "Tulokset tallennettu tiedostoon '"+str(tiedostonnimi1)+"'.\n")

            tiedosto.write("Pienin jätekuorma tuli "+str(datetime.datetime.strftime(tallennus.minpaiva,"%d.%m.%Y"))+" ja oli "+str(int(tallennus.minimi))+" kg.\n"
                           "Suurin jätekuorma tuli "+str(datetime.datetime.strftime(tallennus.maxpaiva,"%d.%m.%Y"))+" ja oli "+str(int(tallennus.maksimi))+" kg.\n"
                           "Pienimmän ja suurimman kuorman toimitusten välissä oli "+str(ero.days)+" päivää.\n"
                           "Analyysijaksolla jätettä tuli yhteensä "+str(int(tallennus.kokonaisuus))+" kg.\n"
                           "Keskimäärin jätekuorma oli "+str(int(tallennus.ka))+" kg.\n")			#kirjoitetaan data tiedostoon


        tiedosto.close()                    #suljetaan tiedosto

    except Exception:                       #virheenkäsittely
        print(": Ei tuloksia. Analysoi data ennen tallennusta.")
        sys.exit(0)

    return None