#include<stdio.h>

/*5 Kademeli program : hijyen, perde, pamuklu, günlük, ekonomik

Seçilen kademedeki özellikler(O dönen yuvarlak tuştan bu beş kademeden birini seçicek

Hijyen modu : 60Derece + 670Devir + 2Saat
Perde modu : 40Derece + 800Devir + 1 saat 30dk
Pamuklu modu : 70Derece 1000Devir +1 saat 10dk
Günlük modu : 30Derece +690Devir 1 saat
Ekonomik modu : 50Derece 400Devir 1 saat 30 dakka

Hangi devir sayıları var :
*400Devir
*670Devir
*690Devir
*800Devir
*1000Devir


Hangi Sıcaklıklar var :
*30Derece
*40Derece
*50Derece
*60Derece
*70Derece


Hangi saat değerleri var :
1 saat
1 saat 10dk
1 saat 30dk
2Saat


*/

/*! typedef enum ile ilgili kısımlardaki değerleri aslında register tablosunda o ilgili 1 byte içine 
ilgili özellik bakımından ne yazılacağını verdiği bigilere göre belirledik.(örnek bir Resmi beraberinde paylaşılacaktır) */

typedef enum {

    devir_400 = 0b00000001, //1
    devir_670 = 0b00000010, //2
    devir_690 = 0b00000011, //3
    devir_800 = 0b00000100, //4
    devir_1000 = 0b00000101,//5
    
}DevirOzellik;

typedef enum {
//m kısmı saatin tam kısmını gösterirken, t kısmı ise ondalıklı tarafını gösterir
    saat_m1_t0 = 0b00000001, //1  ==> 1 saat 0 dakka
    saat_m1_t10= 0b00000010, //2  ==> 1 saat 10 dakka
    saat_m1_t30 = 0b00000011, // ==> 1 saat 30 dakka
    saat_m2_t0 = 0b00000100, //4 ==> 2 saat 0 dakka
    
}SaatOzellik;

typedef enum {

    derece_30 = 0b00000001, //1
    derece_40 = 0b00000010, //2
    derece_50 = 0b00000011, //3
    derece_60 = 0b00000100, //4
    derece_70 = 0b00000101,//5
    
}DereceOzellik;

/* Bu struct sayesinde de örneğin değişken tipi tıpkı "int", "char" gibi fakat kendine has bir tiple 
"DereceOzellik" yapıp DereceVerisi adında bir değişken tanımladık.*/
typedef struct {
	
	DereceOzellik DereceVerisi;
	SaatOzellik   SaatVerisi;
	DereceOzellik DevirVerisi;
	
}Ozellik_Toplayici;


typedef struct {
	
I2cHandleTypedef *i2c;  // Ek özellik olsun diye ekledim 
int makine I2C_ADDRESS; // Ek özellik olsun diye ekledim
char makinedurum;		// Ek özellik olsun diye ekledim

Ozellik_Toplayici Ozellikler; //Buradaki "Ozellikler" değişkeni "DereceOzellik" ile tanımlanan "DereceVerisi"ne ve diğerlerine gidecektir.
																//"SaatOzellik " ile tanımlanan "SaatVerisi"ne ve diğerlerine gidecektir.
															    //Sonrasında içerisine değer atamaları gerçekleşecek
	
}MakineProgramOlusturucu;

int main() {

int a = 12;


MakineProgramOlusturucu program_1; //"program_1 adında bir değişken oluşturdum bu değişken 
 								   //I2cHandleTypedef tipindeki i2c bilgisini tutabilir, int tipindeki ı2c_Address bilgisini tutabilir... Aynı şekilde
								   //Ozellik_Toplayıcı ile tanımlanan Ozellikler adındaki değişkeninde kendi içindeki değişkenlerine erişebilir.Örneğin:
								   
				
				program_1.i2c = &a; // i2c struct içinde pointer ile tanımlandı haliyle & oparatörü ile içerisine adress atayabilirim.
				program_1.makinedurum = 'f' /*Program_1 bir değişken idi ve o değişkenin de kendi alt değişkenleri var(ulaşabileceği) ulaştığı değişkenlerden
											biri de makinedurum değişkeni, buna atayacağı değer ya da bundan çekeceği değeri bu ikisinin birleşiminde saklanır
											yani programın herhangi bir yerinde "makinedurum" nedir sorgusu yaparsam hatalı olur. Ama "program_1.makinedurum"
											nedir sorgusu program_1 değişkenine özel bir atama olup hatta alt dalı olan makinedurum değişkeninin değeri olarak
											atama yapılmıştır.*/ 
				
				
				program_1.Ozellikler.DereceVerisi = derece_30; // (program_1.Ozellikler.DereceVerisi) ifadenin değeri derece_30 yani enumdaki sayısal karşılığı 1 dir
				program_1.Ozellikler.DevirVerisi = devir_800; // (program_1.Ozellikler.DevirVerisi) ifadenin değeri devir_800 yani enumdaki sayısal karşılığı 4 dür
				program_1.Ozellikler.SaatVerisi	= saat_m1_t30;	//	(program_1.Ozellikler.SaatVerisi) ifadenin değeri saat_m1_t30 yani enumdaki sayısal karşılığı 3 dür
			
			
//şimdi buradaki özelliklere verileri atadık ama ne yapıcaz, Diyelim ki 1bytelık bir register olsun(datasheet bize bunu söyleyecek) :
// |7.bit|6.bit|5.bit|4.bit|3.bit|2.bit|1.bit|0.bit| ==> [2:0] bitler derece bilgisine
													//   [5:3] bitler devir bilgisine 
													//   [7:6] biter ise saat bilgisine sahip olsun (tekrar ediyorum bu ayırmada üretici tarafından yapılır.
													
// |x|x|x|x|x|x|x|x| olan registerlara 
//[2:0] tarafına yazılacak olan bilgi derece_30 yani 1 yani 0x00000001 bilgisidir. 
//[5:3] tarafına yazılacak olan bilgi devir_800	yani 4 yani 0x00000100 bilgisidir.						
//[7:6] tarafına yazılacak olan bilgi saat_m1_t30	yani 3 yani 0x00000011 bilgisidir.	
			
			uint8_t registerDegerTutucu = ((program_1.Ozellikler.DereceVerisi<<0) | (program_1.Ozellikler.DevirVerisi<<3) | (program_1.Ozellikler.SaatVerisi)<<6)
			// "<<" oparatörü ilgili 1 ve 0 lardan oluşan değeri 0.bitten itibaren yazmaya başla, 3.bitten itinbaren yazmaya başla ifadesine gelmektedir.
			//daha detaylı bilgi için bitwise oparatörleri izlenmelidir.	   

//Bu bilgiler yazılınca x ler ile gösterilen adres bölgesi şuna döner ==> |1|1|1|0|0|0|0|1| artık bunu sensörün içerisine gönderince o seçilen parametreler
//doğrultusunda işlem yapmaya başlayacaktır.


}