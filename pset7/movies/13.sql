SELECT DISTINCT name
FROM people
JOIN stars ON stars.person_id = people.id
WHERE stars.movie_id IN (SELECT movies.id FROM stars JOIN movies ON stars.movie_id = movies.id JOIN people ON stars.person_id = people.id WHERE people.name = "Kevin Bacon" AND people.birth = 1958) AND people.name != "Kevin Bacon"