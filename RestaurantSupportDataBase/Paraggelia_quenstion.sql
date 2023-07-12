-------------------------------
------Εύρεση της Επόμενης id_paraggelias
/*
select count(*) from PARAGGELIA;
*/
-------------------------------
------Τα τραπέζια που έχουν πάρει απόδειξη
/*
select distinct id_trapezi from PARAGGELIA 
where date_kratisi = '2022-01-07' and
	  wra_kratisi <= '21:15:00' and
	  wra_kratisi_end >= '21:15:00' and 
	  tropos_plirwmis is not NULL;
*/
-------------------------------
------Τα τραπέζια που θέλουν να παραγγείλουν
/*
select id_trapezi from KRATISI 
where akirwsi = 0 and 
	  date_kratisi = '2022-01-07' and
	  wra_kratisi <= '21:15:00' and
	  wra_kratisi_end >= '21:15:00'
except
select distinct id_trapezi from PARAGGELIA 
where date_kratisi = '2022-01-07' and
	  wra_kratisi <= '21:15:00' and
	  wra_kratisi_end >= '21:15:00' and 
	  tropos_plirwmis is not NULL;
*/
-------------------------------
------Τα στοιχεία του σερβιτόρου
/*
select firstname, lastname, amka from ERGAZOMENOS 
inner join SERVITOROS on amka = id_ergazomenou
where posto = 'ΣΕΡΒΙΤΟΡΟΣ';
*/
-------------------------------
------Το μενού του εστιατορίου
/*
select P.onoma, P.timh, KP.onoma, P.id_piato from PIATO as P 
inner join KATHGORIA_PIATOY as KP using(id_katigorias);
*/
-------------------------------
------Τα πιάτα που δεν μπορούν να σερβιριστούν λόγω έλειψης προιόντων
/*
select distinct P.onoma from PIATO as P 
inner join PIATO_HAS_PROIONTA as PP using(id_piato)
inner join PROIONTA as Pr using(id_proiontos)
where Pr.posotita<=1.0;
*/
-------------------------------
------Τα άτομα που υπάρχουν στο τραπέζι
/*
select atoma, date_kratisi, wra_kratisi, wra_kratisi_end, id_trapezi from KRATISI
where date_kratisi = '2022-01-07' and
      wra_kratisi <= '21:15:00' and
      wra_kratisi_end >= '21:15:00' and
      id_trapezi = 4;
*/
-------------------------------
------Εύρεση της Επόμενης id_paraggelias_has_piata
/*
select count(*) from PARAGGELIA_HAS_PIATA;
*/
-------------------------------
------Τα προιόντα με τις ποσοτητές τους
/*
select id_proiontos, posotita from PROIONTA;
*/
-------------------------------
------Το κάθε πιάτο τι ποσοτητα έχει του κάθε προιόντος
/*
select id_piato, id_proiontos, posotita from PIATO_HAS_PROIONTA; 
*/