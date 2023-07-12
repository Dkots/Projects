-------------------------------
------Τα τραπέζια που δεν έχουν πάρει απόδειξη
/*
select distinct K.id_trapezi, K.date_kratisi, K.wra_kratisi from KRATISI as K
inner join PARAGGELIA as P using(date_kratisi, wra_kratisi, wra_kratisi_end, id_trapezi)
where K.akirwsi = 0 and 
      K.date_kratisi = '2022-01-07' and
      K.wra_kratisi <= '21:15:00' and
      K.wra_kratisi_end >= '21:15:00' and
      P.tropos_plirwmis is NULL;
*/
-------------------------------
------Συνολικά ποιά πιάτα έχει παραγγείλει το τραπέζι
/*
select p.onoma, count(*) as total_pos, p.timh*count(*) as axia from PARAGGELIA as par
inner join PARAGGELIA_HAS_PIATA as php using(id_paraggelias)
inner join PIATO as p using(id_piato)
inner join KATHGORIA_PIATOY as kp using(id_katigorias)
where par.id_paraggelias in (select id_paraggelias from PARAGGELIA
							 where date_kratisi = '2022-01-07' and
								   wra_kratisi <= '21:15:00' and
								   wra_kratisi_end >= '21:15:00' and
								   id_trapezi = 4 and
								   tropos_plirwmis is NULL)
group by p.onoma
order by kp.id_katigorias;
*/
