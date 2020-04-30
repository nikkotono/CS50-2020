-- list all movies released in 2010 and their ratings, in descending order by rating.
-- For movies with the same rating, order them alphabetically by title.

SELECT movies.title, ratings.rating FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE movies.year IS '2010' AND ratings.rating IS NOT NULL GROUP BY movies.title ORDER BY ratings.rating DESC;
