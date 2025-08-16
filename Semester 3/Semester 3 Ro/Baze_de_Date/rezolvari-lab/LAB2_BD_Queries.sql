GO
USE Facultateea
GO

SELECT * FROM Cursuri

--afiseaza cursurile care sunt in orarul grupei 223
SELECT DISTINCT C.Denumire, C.Descriere
FROM Orar O INNER JOIN Cursuri C ON C.Cid=O.Oid
WHERE O.Descrierre='ORAR223'

--afiseaza studentii care au la inceputul numelui "NUME"
SELECT DISTINCT Nume, Prenume FROM Studenti
WHERE Nume like'NUME%'


--afiseaza numele, prenumele sefilor de grupa ordonate dupa nume
SELECT DISTINCT S.Nume, S.Prenume FROM Studenti S RIGHT OUTER JOIN SefDeGrupa SF ON S.Sid = SF.Sfid 

--afiseaza numele si prenumele profesorilor din departamentul de matematica 
SELECT P.Nume, P.Prenume FROM Profesori P RIGHT OUTER JOIN DepartamentProfesori D ON P.Pid=D.Did INNER JOIN Specializare S on D.Did=S.SPid
WHERE S.Denumire = 'MATE'

--afiseaza profesorul care are acelasi id cu al facultatii
SELECT P.Nume, P.Prenume FROM Profesori P INNER JOIN Facultate F ON P.Pid = F.Fid
WHERE F.Denumire='MATEMATICA-INFORMATICA'

--afiseaza toti studentii care au media peste 6
SELECT DISTINCT S.Nume, S.Prenume,S.Media FROM Studenti S
WHERE S.Media > 6
Group by S.Nume,S.Prenume, S.Media


--afiseaza studentii din grupa 223 care au media mai mare decat 5 si mai mare decat media
SELECT S.Nume, S.Media
From Studenti S INNER JOIN Grupe G on S.Gid=G.Gid
Where G.Denumire='223'
GROUP BY S.Nume, S.Media
HAVING S.Media > (SELECT AVG(S.Media) FROM Studenti S)

--afiseaza toate grupele din anul 2 a  facultatii de matematica
SELECT G.Denumire FROM Grupe G INNER JOIN Specializare S on G.SPid=S.SPid INNER JOIN Facultate F on S.Fid=F.Fid
Where F.Denumire='MATEMATICA-INFORMATICA'
Group by G.Denumire
HAVING G.Denumire like '22%'

--afiseaza studentii din grupa 222 care au media mai mare decat 5 si mai mare decat media
SELECT S.Nume, S.Media
From Studenti S INNER JOIN Grupe G on S.Gid=G.Gid
Where G.Denumire='222'
GROUP BY S.Nume, S.Media
HAVING S.Media > (SELECT AVG(S.Media) FROM Studenti S)

--afiseaza toate grupele din anul 3 a  facultatii de matematica
SELECT G.Denumire FROM Grupe G INNER JOIN Specializare S on G.SPid=S.SPid INNER JOIN Facultate F on S.Fid=F.Fid
Where F.Denumire='MATEMATICA-INFORMATICA'
Group by G.Denumire
HAVING G.Denumire like '23%'

