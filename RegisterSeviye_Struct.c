#include<stdio.h>

/*
 Çamaşır makinesi; hijyen, perde, pamuklu, günlük, ekonomik ve kapalı modlarından oluşuyor.
 Bu modlar da kendi içinde 3 farklı parametreden oluşuyor. Bu parametreler de sıcaklık, devir ve saat bilgileridir.
 Kullanıcı makinenin yuvarlak tuşunu çevirirken yukarıda sayılan 6 moddan birini seçmek zorunda.
*/


/*
Her modun kendine özgü bir sıcaklık,devir ve saat bilgileri vardır. İşte onlar :

Hijyen modu : 60Derece + 670Devir + 2Saat
Perde modu : 40Derece + 800Devir + 1 saat 30dk
Pamuklu modu : 70Derece 1000Devir +1 saat 10dk
Günlük modu : 30Derece +690Devir 1 saat
Ekonomik modu : 50Derece 400Devir 1 saat 30 dakka
*/

/*
Şimdi yukarıdaki verileri gruplandıralım :

Hangi devir sayıları var ? :
*400Devir
*670Devir
*690Devir
*800Devir
*1000Devir

Hangi Sıcaklıklar var ?:
*30Derece
*40Derece
*50Derece
*60Derece
*70Derece


Hangi saat değerleri var ?:
1 saat
1 saat 10dk
1 saat 30dk
2Saat
*/



/* 
typedef enum ile ilgili kısımlardaki isimlerin karşısındaki değerleri kendimiz belirlemedik
İlgili ürünün datasheetindeki register tablosuna bakınca ilgili register adresinin byte'nda 
ilgili bitlere hangi değerler girileceği bize verildi

Bununla ilgili bir resim bu klasörün içerisinde olacaktır. Fakat resim basınç sensörünün 
özelliği ile ilgili olsa da yaptığınız iş bir bakıma benzerdir.
*/
typedef enum {

    devir_400 = 0b00000001, //1
    devir_670 = 0b00000010, //2
    devir_690 = 0b00000011, //3
    devir_800 = 0b00000100, //4
    devir_1000 = 0b00000101,//5
    
}DevirOzellik;

typedef enum {

	//m kısmı saatin tam kısmını gösterirken, t kısmı ise ondalıklı tarafını gösterir örneğin saat_m1_t0 ==> 1 saat 0 dk bilgisini verir
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

/* 
Bu struct sayesinde de örneğin değişken tipi tıpkı "int", "char" gibi. Fakat kendine has bir tiple 
"DereceOzellik" ile tanımlanıp DereceVerisi adında bir değişken oluşturduk.
*/
typedef struct {
	
	DereceOzellik DereceVerisi;
	SaatOzellik   SaatVerisi;
	DevirOzellik DevirVerisi;
	
}Ozellik_Toplayici;


typedef struct {
	
I2cHandleTypedef *i2c;  // Ek özellik olsun diye ekledim
int makine I2C_ADDRESS; // Ek özellik olsun diye ekledim
char makinedurum;		// Ek özellik olsun diye ekledim



Ozellik_Toplayici Ozellikler; /*
								Buradaki "Ozellikler" değişkeni 
								"DereceOzellik" ile tanımlanan "DereceVerisi"ne
								"SaatOzellik " ile tanımlanan "SaatVerisi"ne
								"DevirOzellik" ile tanımlanan "DevirVerisi"ne 
								ulaşıp tek bir değiken oluşturacaktır bu değişken de en son ne ile bittiyse ilgili değerleri oradan toplayacaktır.
								*/ 
	
}MakineProgramOlusturucu; 

int main() {

int a = 0xF7; //program_1.i2c 'nin içerisine adres ataması yapmak için oluşturdum.


MakineProgramOlusturucu program_1; /*
									program_1 adında bir değişken oluşturdum bu değişken 
									I2cHandleTypedef tipindeki i2c bilgisini tutabilir, int tipindeki ı2c_Address bilgisini tutabilir... Aynı şekilde
									"Ozellik_Toplayıcı" ile tanımlanan "Ozellikler" adındaki değişkenine erişip o değişkeninde kendi içerisinde tuttuğu
									değişkenlerine(DereceVerisi, SaatVerisi, DevirVerisi)erişebilir.
									(Bir ağaç düşünün en kalın dalından en inceye doğru bir yol çiziyoruz ram üstünde)
									*/ 
								   				
	program_1.i2c = &a;			// i2c, struct içinde pointer ile tanımlandı. Haliyle & oparatörü ile içerisine adress atayabilirim.
				
	program_1.makinedurum = 'f' /*Program_1 bir değişken idi ve o değişkenin de kendi alt değişkenleri var(ulaşabileceği). Ulaştığı değişkenlerden
								biri de makinedurum değişkeni, değişkene atayacağı değer ya da değişkenden çekeceği değeri bu ikisinin birleşiminde saklanır
								yani programın herhangi bir yerinde "makinedurum" nedir sorgusu yaparsam hatalı olur. Ama "program_1.makinedurum"
								nedir sorgusu program_1 değişkenine özel bir atama doğrudan daha önce içerisine atanan değeri verecektir.*/ 
					
				
			
	program_1.Ozellikler.DereceVerisi = derece_30;		// (program_1.Ozellikler.DereceVerisi) ifadenin değeri derece_30 yani enumdaki sayısal karşılığı 1 dir
	program_1.Ozellikler.DevirVerisi  = devir_800;		// (program_1.Ozellikler.DevirVerisi) ifadenin değeri devir_800 yani enumdaki sayısal karşılığı 4 dür
	program_1.Ozellikler.SaatVerisi	  = saat_m1_t30;	//	(program_1.Ozellikler.SaatVerisi) ifadenin değeri saat_m1_t30 yani enumdaki sayısal karşılığı 3 dür
				
	
/*
Üstteki değişkenlere değerler atadık peki sonra ne yapacağız. Diyelim ki 1bytelık bir register olsun(datasheet bize bunu söyleyecek) :
		|7.bit|6.bit|5.bit|4.bit|3.bit|2.bit|1.bit|0.bit| ==> [2:0] bitler derece bilgisine
															  [5:3] bitler devir bilgisine
															  [7:6] biter ise saat bilgisine sahip olsun

															  |x|x|x|x|x|x|x|x| olan registerlara

															  [2:0] tarafına yazılacak olan bilgi derece_30 yani 1 yani 0x00000001 bilgisidir.
															  [5:3] tarafına yazılacak olan bilgi devir_800	yani 4 yani 0x00000100 bilgisidir.
															  [7:6] tarafına yazılacak olan bilgi saat_m1_t30	yani 3 yani 0x00000011 bilgisidir.

															  Burada akla şu soru gelebilir. 7 ve 6. bite 1 1 koymamız gerekirken neden 0 ve 1. bitlere 1 1 değerleri
															  koyduk ? Ki gerçekten de haklı bir sorudur. Lakin aşağıda bunu neden yaptığımızı uygulamalı olarak 
															  anlatmış olacağım.

*/	

			
			uint8_t registerDegerTutucu = ((program_1.Ozellikler.DereceVerisi<<0) | (program_1.Ozellikler.DevirVerisi<<3) | (program_1.Ozellikler.SaatVerisi)<<6)
			
			/*
			"<<" oparatörü ilgili 1 ve 0 lardan oluşan değeri 0.bitten itibaren yazmaya başla, 3.bitten itinbaren yazmaya başla ifadesine gelmektedir.
			daha detaylı bilgi için bitwise oparatörleri izlenmelidir. Ama yukarıdaki soruya bir cevap olması bakımından 6. ve 7. bitlere 1 1 yazmayıp
			0 ve 1. bitlere [1 1] yazıp bu değeri tutan değişkeni(program_1.Ozellikler.SaatVerisi) yukarıdaki gibi "<<" oparatörü ile ilgili değeri 6 bit kaydırınca
			o değeri oluşturan [1 1] bitleri de 6 bit kayıyor ve 6. bitten itibaren yer kaplamaya başlıyor. 
			*/ 


			HAL_I2C_Mem_Write(&i2c1, 0x77 , 0x10, 1, &registerDegerTutucu, 1, 1000);
			/*
			HAL_I2C_Mem_Write komutu stm32'nin HAL kütüphanesinde bulunup ı2c haberleşmesi yapması üzerine tasarlanmıştır.
			Çalışma yapısında asıl değinmek istediğim şey ==> 0x77 chip'in genel adresi
			*/
			
																
/*
Bu bilgiler yazılınca x ler ile gösterilen adres bölgesi şuna döner ==> |1|1|1|0|0|0|0|1| artık bunu sensörün
içerisine gönderince o seçilen parametreler doğrultusunda işlem yapmaya başlayacaktır.
*/



}