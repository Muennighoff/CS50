SELECT name FROM people join stars ON people.id = stars.person_id JOIN movies ON movies.id = stars.movie_id WHERE title = "Toy Story";