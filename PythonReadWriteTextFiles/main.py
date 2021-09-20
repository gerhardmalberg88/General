import HT_kirjasto

'''
def main():
    print("Debug1")
    HT_kirjasto.valikko()

if __name__ == "__main__":
    main()
'''





def main():

    listaluku = []                          #luodaan listat
    listatallennus = []
            
    try:                                    #virheenkäsittely
        
        while True:
            valinta = HT_kirjasto.valikko()

            if valinta == 1:
            
                tiedostonnimi = input("Anna kuormatietotiedoston nimi: ")
                listaluku = HT_kirjasto.lukeminen(listaluku,tiedostonnimi)

            elif valinta == 2:
                listatallennus = HT_kirjasto.analysoidaan(listaluku,listatallennus)

            elif valinta == 3:
                tiedostonnimi1 = input("Anna tulostiedoston nimi: ")
                HT_kirjasto.tallentaminen(tiedostonnimi1,listatallennus)
            
            elif valinta == 0:
                print("Kiitos ohjelman käytöstä.")
                listaluku.clear()                       #tyhjennetään listat lopuksi
                listatallennus.clear()
                break
            
            else:
                print("Valintaa ei tunnistettu, yritä uudestaan.\n")

    except Exception:                                   #virheenkäsittely
        print("Valintaa ei tunnistettu, yritä uudestaan.\n")
        pass

if __name__ == "__main__":
    main()


# Kirjaston sisällä nämä

class toiminnot:
    def valikko():
        #kirjota tähän sun koodi tälle funktiolle
    def lukeminen():
    #kirjota tähän sun koodi tälle funktiolle
    def analysoidaan():
    #kirjota tähän sun koodi tälle funktiolle
    def tallentaminen():
    #kirjota tähän sun koodi tälle funktiolle

class luku:
    paivat = ""
    arvot = ""
    
class tallentaa:
    maksimi = ""
    minimi = ""
    minpaiva= ""
    maxpaiva = ""
    ka = ""
    kokonaisuus = 0


# Main koodissa nämä

# class toiminnot alustus
esimToiminnotOlio = toiminnot()

# Sitten käytä olioo näin
esimToiminnotOlio.analysoidaan() 