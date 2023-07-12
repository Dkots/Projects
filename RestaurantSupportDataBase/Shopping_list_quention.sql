-------------------------------
------Λίστα αγορών
select id_proiontos, P.onoma, K.onoma, posotita from PROIONTA as P
inner join KATHGORIA_PROIONTOS as K using(id_katigorias)
where posotita <= 10.0;