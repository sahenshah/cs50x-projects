import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks."""
    # Get the user's ID from the session
    user_id = session["user_id"]

    # Query the database to get the user's cash balance
    user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = user[0]["cash"]

    # Query the database to get the user's stock holdings
    portfolio = db.execute(
        """
        SELECT symbol, SUM(shares) as total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
        """,
        user_id,
    )

    # Initialize variables for total portfolio value
    total_value = cash

    # Look up the current price of each stock and calculate the total value of each holding
    for stock in portfolio:
        symbol = stock["symbol"]
        shares = stock["total_shares"]
        stock_info = lookup(symbol)
        if stock_info:
            stock["name"] = stock_info["name"]
            stock["price"] = stock_info["price"]
            stock["total_value"] = shares * stock_info["price"]
            total_value += stock["total_value"]
        else:
            # Handle case where stock lookup fails
            stock["name"] = "N/A"
            stock["price"] = 0
            stock["total_value"] = 0

    # Render the index template with the portfolio data
    return render_template("index.html", portfolio=portfolio, cash=cash, total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    user_id = session["user_id"]

    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate the symbol
        if not symbol:
            return apology("Please enter a stock symbol.")

        stock_info = lookup(symbol)
        if not stock_info:
            return apology("Invalid stock symbol.")

        # Validate the number of shares
        try:
            shares = int(shares)
            if shares < 1:
                return apology("Please enter a positive integer.")
        except ValueError:
            return apology("Please enter a valid number of shares.")

        # Calculate total cost of Shares
        total_cost = shares * stock_info["price"]

        # Query the database to get the user's cash balance
        user = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = user[0]["cash"]

        # Check if the user can afford the purchase
        if total_cost > cash:
            return apology("You cannot afford this purchase.")

        # Deduct the total cost from the user's cash balance
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_cost, user_id)

        #Add transaction to table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol.upper(),
            shares,
            stock_info["price"],
            datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        )

        # Redirect user to home page
        return redirect("/")

    else:
        # If it's a GET request, render the buy form
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    # Get the user's ID from the session
    user_id = session["user_id"]

    # Query the database to get all transactions for the user
    transactions = db.execute(
        """
        SELECT symbol, shares, price, timestamp
        FROM transactions
        WHERE user_id = ?
        ORDER BY timestamp DESC
        """,
        user_id,
    )

    # Add a "type" field to indicate whether the transaction was a buy or sell
    for transaction in transactions:
        transaction["type"] = "Buy" if transaction["shares"] > 0 else "Sell"
        transaction["shares"] = abs(transaction["shares"])  # Display absolute number of shares


    """Show history of transactions"""
    return render_template("history.html", transactions=transactions)


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
    if request.method == "POST":
        symbol = request.form.get("symbol")

        # Validate the symbol
        if not symbol:
            return apology("Please enter a stock symbol.")

        # Look up the stock price using the lookup function
        stock = lookup(symbol)

        # Check if the stock was found
        if not stock:
            return apology("Invalid stock symbol.")

        # Render the quoted template with the stock information
        return render_template("quoted.html", stock=stock)

    else:
        # If it's a GET request, render the quote form
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # Get form data
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        if not password:
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        if not confirmation:
            return apology("must confirm password", 400)

        # Ensure passwords match
        if password != confirmation:
            return apology("passwords do not match", 400)

        # Hash the password
        hash = generate_password_hash(password)

        # Query database for username
        rows = db.execute( "SELECT * FROM users WHERE username = ?", username )

        # Ensure username does not already exist
        if len(rows) > 0:
            return apology("Username Already Exists", 400)

         # Insert data into database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        return redirect("/")
    else:
        # Render the registration form for GET requests
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_id = session["user_id"]

    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Validate the symbol
        if not symbol:
            return apology("Please enter a stock symbol.")

        stock_info = lookup(symbol)
        if not stock_info:
            return apology("Invalid stock symbol.")

        # Query the database to get the user's stock holdings
        portfolio = db.execute(
            """
            SELECT symbol, SUM(shares) as total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
            """,
            user_id,
        )

        # Check if the user owns any shares of the selected stock
        owned_shares = next((stock["total_shares"] for stock in portfolio if stock["symbol"] == symbol), 0)
        if owned_shares < 1:
            return apology(f"You do not own any shares of {symbol}.")

        # Validate the number of shares
        try:
            shares = int(shares)
            if shares < 1:
                return apology("Please enter a positive integer.")
            elif shares > owned_shares:
                return apology(f"You do not own {shares} shares of {symbol}.")
        except ValueError:
            return apology("Please enter a valid number of shares.")

        # Calculate total cost of Shares
        total_proceeds = shares * stock_info["price"]


        # Increment the total cost from the user's cash balance
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", total_proceeds, user_id)

        #Add transaction to table
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price, timestamp) VALUES (?, ?, ?, ?, ?)",
            user_id,
            symbol.upper(),
            -shares,
            stock_info["price"],
            datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        )

        # Redirect user to home page
        return redirect("/")

    else:
        # If it's a GET request, render the sell form
        # Get the user's portfolio to populate the dropdown menu
        portfolio = db.execute(
            """
            SELECT symbol, SUM(shares) as total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
            """,
            user_id,
        )

        return render_template("sell.html", portfolio=portfolio)
