-- list the titles of all movies with a release date on or after 2018, in alphabetical order.
SELECT movies.title FROM movies WHERE year >= 2018 GROUP BY movies.title ORDER BY movies.title ASC;