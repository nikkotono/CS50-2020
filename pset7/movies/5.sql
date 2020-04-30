-- list the titles and release years of all Harry Potter movies, in chronological order
SELECT movies.title,movies.year FROM movies WHERE title LIKE '%harry potter%' GROUP BY year ORDER BY year ASC;