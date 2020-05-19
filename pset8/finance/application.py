import os
import time as time2
from datetime import datetime, time

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Check if US markets are open
def is_time_between(begin_time, end_time, check_time=None):
    # If check time is not given, default to current UTC time
    check_time = check_time or datetime.utcnow().time()
    print("Check_Time: ", check_time)
    if begin_time < end_time:
        return check_time >= begin_time and check_time <= end_time
    else: # crosses midnight
        return check_time >= begin_time or check_time <= end_time

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get the amount of cash & set initial total equal to cash
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
    total = cash

    # Get the shares of each symbol from users portfolio
    list_dict = db.execute("SELECT symbol, shares FROM portfolio WHERE users_id = :id", id=session["user_id"])

    # Iterate over the list dict and adapt to the needed format
    for i in range(0, len(list_dict)):
        list_dict[i]["name"] = lookup(list_dict[i]["symbol"])["name"]
        list_dict[i]["price"] = lookup(list_dict[i]["symbol"])["price"]
        list_dict[i]["total"] = list_dict[i]["price"] * list_dict[i]["shares"]
        # Updating the total
        total += list_dict[i]["total"]

    print("List_Dict: ", list_dict)


    # Change all to USD tags and round to two decimals
    for i in range(0, len(list_dict)):
        list_dict[i]["total"] = usd(list_dict[i]["total"])
        list_dict[i]["price"] = usd(list_dict[i]["price"])

    total = usd(round(total, 2))
    cash = usd(round(cash, 2))

    return render_template("index.html", list_dict=list_dict, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # Submitting a buy?
    if request.method == "POST":

       # Ensure markets are open
        if is_time_between(time(13,30), time(20,00)) == False:
            return apology("US markets are closed.", 400)


        # Check if symbol is given:
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Check if number is given:
        if not request.form.get("shares"):
            return apology("must provide amount", 400)

        # Sum of money intended to buy & storing variables for efficiency
        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))
        # To account for & etc. like symbols
        try:
            price = lookup(symbol)["price"]
        except:
            return apology("Invalid symbol", 400)

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        # Total buy amount
        total = shares * price

        # Enough cash to buy stocks?
        if total > cash:
            return apology("not enough money in your balance", 400)

        # Purchase the stock and decrease balance
        else:
            # Update Cash balance
            db.execute("UPDATE users SET cash = :new_cash WHERE id = :id", new_cash=(cash-total), id=session["user_id"])

            # Add transaction to the database of transactions (transaction log)
            db.execute("INSERT INTO transactions (users_id, symbol, price, shares, time ) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, price, shares, time2.strftime('%Y-%m-%d %H:%M:%S'))

            # Add transaction to users holdings
            holding = db.execute("SELECT * FROM portfolio WHERE users_id = ? AND symbol = ?", session["user_id"], symbol)

            # Check if the stock is already in the portfolio
            if len(holding) == 0:
                # Insert the holding
                db.execute("INSERT INTO portfolio (users_id, symbol, shares) VALUES (?, ?, ?)", session["user_id"], symbol, shares)
            else:
                # Update the holding
                db.execute("UPDATE portfolio SET shares = shares + ? WHERE users_id = ? AND symbol = ?", shares, session["user_id"], symbol)

            # Display confirmation message
            return redirect("/")

    # Trying to get the page
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Select all transactions from the user

    list_dict = db.execute("SELECT symbol, price, shares, time FROM transactions WHERE users_id = ? ORDER BY time DESC", session["user_id"])

    return render_template("history.html", list_dict=list_dict)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If reached via POST:
    if request.method == "POST":

        # Check if input given:
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # If input is given
        else:
            dict_lookup = lookup(request.form.get("symbol"))

            if dict_lookup is not None:
                return render_template("lookup.html", name = dict_lookup["name"], price = usd(dict_lookup["price"]))

            else:
                return apology("Invalid symbol", 400)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

     # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username submit
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password submit
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure 2nd password submit
        elif not request.form.get("password"):
            return apology("must type password again", 400)

        # Ensure passwords are equal
        elif request.form.get("password") != request.form.get("password2"):
            return apology("passwords must be the same", 400)

        # Ensure password contains 6-20 letters
        elif not 4 < len(request.form.get("password")) < 40:
            return apology("password must be between 4 and 40 chars", 400)

        # Also check if no existing user with the same username!
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) != 0:
            return apology("username already taken", 400)

        # Add new user to the database if all correct
        else:
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))
            return redirect("/")


    # If GET request method
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Get the portfolio of the current user
    list_dict = db.execute("SELECT symbol, shares FROM portfolio WHERE users_id = ?", session["user_id"])

    # If reached out via POST:
    if request.method == "POST":

        # Ensure markets are open
        if is_time_between(time(13,30), time(20,00)) == False:
            return apology("US markets are closed.", 400)

        # Ensure symbol is not empty
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure shares is not empty
        elif not request.form.get("shares"):
            return apology("must provide amount", 400)

        # Identify the symbol they want to sell:
        for i in range(0, len(list_dict)):

            if list_dict[i]["symbol"] == request.form.get("symbol"):

                if not int(request.form.get("shares")) <= list_dict[i]["shares"]:
                    return apology("you do not own that many shares", 400)

                # If amount is okay let's set some variables & execute the sell:
                shares = int(request.form.get("shares"))
                symbol = request.form.get("symbol").upper()
                price = lookup(symbol)["price"]

                # Add transaction to the database of transactions (transaction log)
                db.execute("INSERT INTO transactions (users_id, symbol, price, shares, time ) VALUES (?, ?, ?, ?, ?)", session["user_id"], symbol, price, shares * (-1), time2.strftime('%Y-%m-%d %H:%M:%S'))

                # Update portfolio
                db.execute("UPDATE portfolio SET shares = shares - ? WHERE users_id = ? AND symbol = ?", shares, session["user_id"], symbol)

                # Update cash balance
                db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", price*shares, session["user_id"])

                # Check whether shares remain
                rem_shares = db.execute("SELECT shares FROM portfolio WHERE users_id = ? AND symbol = ?", session["user_id"], symbol)

                # If no shares left delete holding from portfolio
                if rem_shares[0]["shares"] == 0:
                    db.execute("DELETE FROM portfolio WHERE users_id = ? AND symbol = ?", session["user_id"], symbol)

                return redirect("/")

    # If method is GET
    else:
        return render_template("sell.html", list_dict=list_dict)


@app.route("/ranking")
@login_required
def ranking():
    """Rank users"""

    # Get the amount of cash & names & ids for each user
    list_dict = db.execute("SELECT cash, username, id FROM users")


    for i in range(0, len(list_dict)):
        # Create a total key and initialize to cash balance
        list_dict[i]["total"] = list_dict[i]["cash"]
        # Get the users portfolio
        portfolio = db.execute("SELECT symbol, shares FROM portfolio WHERE users_id = ?", list_dict[i]["id"])

        # Add portfolio value to total
        for j in range(0, len(portfolio)):

            list_dict[i]["total"] += lookup(portfolio[j]["symbol"])["price"] * portfolio[j]["shares"]


    # Double for loop to determine rankings
    for i in range(0, len(list_dict)):
        # Initialize rank as 1
        list_dict[i]["rank"] = 1

        # Check for better ranked portfolios
        for j in range(0, len(list_dict)):
            if list_dict[i] != list_dict[j]:
                if list_dict[i]["total"] < list_dict[j]["total"]:
                    list_dict[i]["rank"] += 1

    print(list_dict)

    # Sort the list by ranks:
    list_dict.sort(key=lambda x: x["rank"])


    # def take_rank(list)
     #   return list["rank"]


    return render_template("ranking.html", list_dict=list_dict)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
