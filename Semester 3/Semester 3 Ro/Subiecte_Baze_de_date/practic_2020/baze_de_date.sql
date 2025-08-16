create database DB
go
use DB
go

-- 1) create tables and insert values into them

create table Papusa(
	cod_p int PRIMARY KEY IDENTITY,
	nume VARCHAR(100),
	gen varchar(100),
	prestanta varchar(100),

)

insert into Papusa(nume, gen, prestanta) values ('papusa1', 'feminin','foarte bine');
insert into Papusa(nume, gen, prestanta) values ('papusa2', 'masculin','excelent');
insert into Papusa(nume, gen, prestanta) values ('papusa3', 'maculin','bine');
insert into Papusa(nume, gen, prestanta) values ('papusa4', 'maculin','bine');
insert into Papusa(nume, gen, prestanta) values ('papusa5', 'maculin','bine');

create table Teatru(
	cod_t int primary key identity,
	nume varchar(100),
	oras varchar(100),
	nrdestele int
)

insert into Teatru (nume, oras, nrdestele) values ('teatru1','cluj-napoca',3);
insert into Teatru (nume, oras, nrdestele) values ('teatru2','cluj-napoca',4);
insert into Teatru (nume, oras, nrdestele) values ('teatru3','cluj-napoca',5);

create table Mascota(
	cod_m int primary key identity,
	nume varchar(100),
	culoarepreferata varchar(100),
	varsta int
)

insert into Mascota (nume, culoarepreferata, varsta) values ('mascota1','rosu',19);
insert into Mascota (nume, culoarepreferata, varsta) values ('mascota2','albastru',20);
insert into Mascota (nume, culoarepreferata, varsta) values ('mascota3','galben',21);

select * from Teatru
select * from Mascota
select * from Papusa

create table Spectacol(
	cod_s int primary key identity,
	denumire varchar(100),
	durata varchar(100),
	numeregizor varchar(100),
	cod_t int foreign key references Teatru(cod_t),
	cod_m int foreign key references Mascota(cod_m)
)

insert into Spectacol (denumire, durata, numeregizor, cod_t, cod_m) values ('spectacor1', '1 ore', 'regizor1',1,1)
insert into Spectacol (denumire, durata, numeregizor, cod_t, cod_m) values ('spectacor2', '2 ore', 'regizor2',2,2)
insert into Spectacol (denumire, durata, numeregizor, cod_t, cod_m) values ('spectacor3', '3 ore', 'regizor3',3,3)
insert into Spectacol (denumire, durata, numeregizor, cod_t, cod_m) values ('spectacor4', '4 ore', 'regizor4',3,3)
insert into Spectacol (denumire, durata, numeregizor, cod_t, cod_m) values ('spectacor5', '5 ore', 'regizor5',3,3)

select * from Spectacol
select * from Papusa

create table Spectacol_Papusa(
	cod_s int foreign key references Spectacol(cod_s),
	cod_p int foreign key references Papusa(cod_p),
	data varchar(100),
	pretbilet int ,
	constraint cod_sp primary key (cod_s,cod_p)
)

insert into Spectacol_Papusa(cod_s, cod_p, data, pretbilet) values (1,1,'13-ianuarie-2020',15)
insert into Spectacol_Papusa(cod_s, cod_p, data, pretbilet) values (2,2,'14-ianuarie-2020',20)
insert into Spectacol_Papusa(cod_s, cod_p, data, pretbilet) values (3,3,'15-ianuarie-2020',25)

select * from Spectacol_Papusa

select * from Papusa
select * from Teatru
select* from Mascota
select * from Spectacol
select * from Spectacol_Papusa

--2)

go
create function exista( @cod_s int, @cod_p int)
	returns int
	as begin
	if((select count (*) from Spectacol_Papusa where cod_s = @cod_s and cod_p = @cod_p) > 0)
		return 1
	else
		return 0
	return 0
end

go

create procedure inserare(@cod_p int, @cod_s int, @data varchar(100), @pretbilet int)
	as begin
	if(dbo.exista (@cod_s, @cod_p) = 1) begin
		update Spectacol_Papusa
			set
			data = @data,
			pretbilet = @pretbilet
		where
			cod_s = @cod_s and cod_p = @cod_p
	end
	else begin
		insert into Spectacol_Papusa(cod_s, cod_p, data, pretbilet) values (@cod_s, @cod_p, @data, @pretbilet)
	end
	end

select * from Spectacol_Papusa

exec dbo.inserare 3,3,"17-ianuarie-2020", 35
exec dbo.inserare 5,7,"17-ianuarie-2020", 35

-- 3)

create view vw_Spectacole as
	select S.denumire, T.cod_t from Teatru T
		inner join Spectacol S on T.cod_t = S.cod_t

select * from vw_Spectacole


--4)

create function afisare(@N int)
	returns table as 
		return
			select P.nume, count(*) as NumarAparitii
			from Spectacol_Papusa SP inner join Papusa P on SP.cod_p = P.cod_p
			group by P.cod_p, P.nume
			having count(*) >=@N

select * from afisare(1)
go	