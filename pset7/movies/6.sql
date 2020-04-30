-- determine the average rating of all movies released in 2012
SELECT AVG(ratings.rating) AS 'Average Rating' FROM movies JOIN ratings ON movies.id = ratings.movie_id WHERE year IS '2012';