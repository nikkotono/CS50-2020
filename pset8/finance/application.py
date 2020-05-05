import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":

        # get user data
        userId   = session["user_id"]
        userRows = db.execute("SELECT * from users WHERE id = :userId",userId=userId)

        bank = 0

        for items in userRows:
            username = items.get("username")
            userId   = items.get("id")
            cash     = items.get("cash")
            bank     += cash
            cash     = usd(cash)

        # get quotes buy transaction data
        sumQuotes = db.execute(
        'SELECT symbol, SUM(shares) as total_shares, cost as total_cost FROM transactions WHERE user_id = :userId AND type = "BUY" GROUP BY symbol',userId=userId
        )

        soldShares = db.execute(
        "SELECT symbol, SUM(shares) as sold_shares, SUM(cost) as sold_total_cost FROM transactions WHERE user_id = :userId AND type = 'SELL' GROUP BY symbol", userId=userId
        )

        # initialize to avoid incorrect sums
        holdings = 0
        worth = 0
        total_worth = 0
        shares = 0
        stockList = []

        # crunch quote numbers
        for items in sumQuotes:
            worth  = 0
            symbol = items["symbol"]
            shares = items["total_shares"]

            # amount of total cost paid for all shares
            holdings = holdings + items["total_cost"] * shares

            worthRows = lookup(items["symbol"])
            stockList = worthRows.items()

            # look at current pricing of stock
            for row in stockList:
                if row[0] == "symbol":
                    symbol = symbol
                if row[0] == "price":
                    worth += row[1]

            # tally total worth of each share
            total_worth += worth * shares

        print(sumQuotes)
        # count sold shares
        for items in soldShares:
            print(items)

        # set precision
        total = bank + holdings
        total = usd(total)
        print("cash: " + str(bank))
        print("holdings: " + str(holdings))
        print("total: " + str(total))
        total_worth = usd(total_worth)
        holdings    = usd(holdings)

        for items in sumQuotes:
            items['total_cost'] = usd(items['total_cost'])

        return render_template("index.html",sumQuotes=sumQuotes, username=username, cash=cash,holdings=holdings, total_worth=total_worth, total=total)
    else:
        # Redirect user back home
        return redirect("/")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    elif request.method =="POST":
        # get symbol for stock we want to purchase
        stock  = request.form.get("symbol")

        # get input and handle corner cases
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Share mut be a number greater than 0")

        if shares < 0:
            return apology("Minimum 1 share to buy")
        if lookup(stock) == None:
            return apology("Choose a new stock", "Invalid stock ticker")

        # tokenize stock info
        stocks = []
        stocks = lookup(stock)
        stockList = stocks.items()
        for items in stockList:
            if items[0] == "price":
                price = items[1]
            elif items[0] == "symbol":
                symbol = items[1]

        # get time of transaction
        time = datetime.utcnow()

        # get current cash from current user
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
        # print(rows)
        # print(rows[0])
        currentCash = float(rows[0].get("cash"))
        purchase = float(price) * float(shares)
        newCash = float(currentCash) - float(purchase)

        # set transaction in database
        if purchase < currentCash:
            db.execute("INSERT INTO transactions (user_id, symbol, cost, time, shares, type) VALUES (:user_id,:symbol,:cost,:time,:shares,'BUY')",user_id=session["user_id"],symbol=symbol,cost=price,time=time,shares=shares)
            db.execute("UPDATE users SET cash = :newCash WHERE id=:user_id",newCash=newCash,user_id=session["user_id"])
        else:
            return apology("Not enough cash for purchase")


    return render_template("buy.html", price=price, symbol=symbol, cost=price, time=time, shares=shares)




@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    history = db.execute("SELECT * FROM transactions WHERE user_id = :user_id",user_id=session["user_id"])
    print(history)
    return render_template("history.html",history=history)


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
    if request.method == "GET":
        return render_template("quote.html")

    elif request.method == "POST":
        stock  = request.form.get("quoteSymbol")
        stocks = []

        if lookup(stock) == None:
            return apology("Choose a new stock", "Invalid stock ticker")

        stocks = lookup(stock)
        stockList = stocks.items()

        for items in stockList:
            if items[0] == "name":
                name = items[1]
            elif items[0] == "price":
                price = items[1]
                price = usd(price)
            elif items[0] == "symbol":
                symbol = items[1]
        # print(name, price, symbol)

    return render_template("quoted.html", name=name, price=price, symbol=symbol)
    # return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        name = request.form.get("username")
        password = request.form.get("password")
        passConfirm = request.form.get("password2")

        if password != passConfirm:
            return apology("Passwords dont match!")
        else:
            db.execute("INSERT INTO users (username, hash, cash) VALUES (:username,:hash,:cash)",username=name, hash=generate_password_hash(password),cash=10000)
            return redirect("/")


@app.route("/bank", methods=["GET", "POST"])
@login_required
def bank():
    if request.method == "GET":
        return render_template("bank.html")
    else:
        funds = request.form.get("funds")

        try:
            funds = int(funds)
        except:
            return apology("funds must be only digits")

        if funds <= 0:
            return apology("You must fund more than 0","Fund error")
        else:
            rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
            currentCash = float(rows[0].get("cash"))
            newCash = float(currentCash) + float(funds)
            db.execute("UPDATE users SET cash = :newCash WHERE id=:user_id",newCash=newCash,user_id=session['user_id'])
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
            return render_template("sell.html")

    elif request.method == "POST":
        # get symbol for stock we want to purchase
        stock  = request.form.get("symbol")

        # get input and handle corner cases
        try:
            sharesToSell = int(request.form.get("shares"))
        except:
            return apology("Share mut be a number greater than 0")

        if sharesToSell < 0:
            return apology("Minimum 1 share to buy")
        if lookup(stock) == None:
            return apology("Choose a new stock", "Invalid stock ticker")

        # tokenize stock info
        stocks = []
        stocks = lookup(stock)
        stockList = stocks.items()
        for items in stockList:
            if items[0] == "price":
                price = items[1]
            elif items[0] == "symbol":
                symbol = items[1]

        # get time of transaction
        time = datetime.utcnow()

        # get current cash from current user
        cashRows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        currentCash = float(cashRows[0].get("cash"))
        purchase    = float(price) * float(sharesToSell)
        newCash     = float(currentCash) + float(purchase)

        stockRows = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])

        sellStock = stock.upper()
        numOfStock = 0
        for row in stockRows:
            if row["symbol"] == sellStock:
                numOfStock += row["shares"]

        print(sellStock)
        print(numOfStock)
        print(sharesToSell)

        newNumOfStock = numOfStock - sharesToSell

        # set transactions in database
        if sharesToSell < numOfStock:
            db.execute("UPDATE users SET cash = :newCash WHERE id=:user_id", newCash=newCash,user_id=session["user_id"])
            db.execute("INSERT INTO transactions (user_id, symbol, cost, time, shares, type) VALUES (:user_id,:symbol,:cost,:time,:shares, 'SELL')",user_id=session["user_id"],symbol=symbol,cost=price,time=time,shares=sharesToSell)
        else:
            apology("Not enough shares to sell")


    return render_template("sell.html", price=price, symbol=symbol, cost=price, time=time, sharesToSell=sharesToSell)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
