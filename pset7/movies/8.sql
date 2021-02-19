SELECT name
FROM stars
JOIN people ON person_id = people.id
JOIN movies ON movie_id = movies.id
WHERE title = "Toy Story"