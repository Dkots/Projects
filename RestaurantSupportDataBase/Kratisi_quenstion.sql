-------------------------------
------Τοποθεσία Τραπεζιών
/*
select onoma from TOPOTHESIA;
*/

-------------------------------
------Εύρεση Γνωστών Πελατών
/*
select id_pelath, firstname, lastname, thlefwno from PELATHS 
order by lastname;
*/
-------------------------------
------Εύρεση του Επόμενου id_pelati
/*
select count(*) from PELATHS;
*/
-------------------------------
------Εύρεση των ελεύθερων τραπεζιών
/*
select id_trapezi, onoma, xwritikothta from TRAPEZI 
inner join TOPOTHESIA using(id_topothesia)
where id_trapezi not in (select id_trapezi from KRATISI 
							where akirwsi = 0 and 
							date_kratisi = '2022-01-07' and 
							wra_kratisi < '23:45:00' and
                            '21:45:00' < wra_kratisi_end) and 
	onoma = 'ΣΑΛΟΝΙ_ΚΕΝΤΡΟ' and
	xwritikothta >= 4;
*/
-------------------------------
------Εύρεση των κρατημένων τραπεζιών
/*
select id_pelath, firstname, lastname, id_trapezi, atoma from PELATHS
inner join KRATISI using(id_pelath)
inner join TRAPEZI using(id_trapezi)
where akirwsi = 0 and 
	date_kratisi = '2022-01-07'  and 
	wra_kratisi >= '21:00:00';
*/

