SELECT DISTINCT(people.name)
FROM stars
JOIN people ON person_id = people.id
JOIN movies ON movie_id = movies.id
WHERE movies.year = 2004
ORDER BY birth