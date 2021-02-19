import os

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

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user data:
    shares = db.execute("SELECT * FROM shareholders WHERE user_id=:user_id", user_id=session["user_id"])
    cash = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])

    # Total money variable
    total = cash[0]["cash"]

    # Get value of each stock
    stock_value = {}
    for share in shares:
        price = lookup(share["stock"])
        stock_value.update({share["stock"]:price["price"]})

    # Get total value of each stock
    stock_total = {}
    for share in shares:
        price = lookup(share["stock"])
        stock_total.update({share["stock"]:round(price["price"]*share["amount"], 2)})
        # Update user account total
        total = total + stock_total[share["stock"]]

    # Render template passing data
    return render_template("index.html", shares=shares, cash=round(cash[0]["cash"], 2), stock_value=stock_value, stock_total=stock_total, total=round(total, 2), username=cash[0]["username"])

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Get Method - Load page
    if request.method == "GET":
        return render_template("buy.html")

    # Post Method - Check if requierements are correct and procced
    if request.method == "POST":
        stock = request.form.get("stock")
        data = lookup(stock)

        if data is None:
            return render_template("error.html", error="Stock not found")
        else:
            amount = float(request.form.get("amount"))
            total = data["price"] * amount

            # Check if user can afford transaction
            account_balance = db.execute("SELECT cash FROM users WHERE id=:number", number=session["user_id"])
            if account_balance[0]["cash"] < total:
                return render_template("error.html", error="Account doesn't have enough funds")

            else:
                # Submit transaction to a history log
                db.execute("INSERT INTO history (user_id, stock, amount, total) VALUES(:user_id, :stock, :amount, :total)", user_id=session["user_id"], stock=stock, amount=amount, total=total)

                # Take funds from account and also add shares

                # Change value of user table -> cash
                db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=account_balance[0]["cash"] - total, user_id=session["user_id"])

                # Add stock ammount into shareholders table

                # Check if user already own stocks
                shares = db.execute("SELECT * FROM shareholders WHERE user_id = :user_id AND stock = :stock_name", user_id=session["user_id"], stock_name=stock)

                # User doesn't own shares
                if not shares:
                    db.execute("INSERT INTO shareholders (user_id, stock, amount) VALUES(:user_id, :stock, :amount)", user_id=session["user_id"], stock=stock, amount=amount)

                # User already owns shares
                else:
                    db.execute("UPDATE shareholders SET amount = :new_amount WHERE user_id = :user_id AND stock = :stock_name", new_amount = shares[0]["amount"] + amount, user_id=session["user_id"], stock_name=stock)

                return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Load user past transactions
    history = db.execute("SELECT * FROM history WHERE user_id=:user_id", user_id=session["user_id"])
    return render_template("history.html", history=history)



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
    # Get Method - Quote page
    if request.method == "GET":
        return render_template("quote.html")

    # Post Method - Lookup stock
    if request.method == "POST":
        stock = request.form.get("stock")
        data = lookup(stock)
        if data is not None:
            return render_template("quoted.html", stock=stock, company=data["name"], value=round(data["price"], 2))
        else:
            return render_template("error.html", error="Stock not found")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Get Method - Registration page
    if request.method == "GET":
        return render_template("register.html")

    # Post Method - Register user
    else:
        # Check if user already exists
        used_names = db.execute("SELECT * FROM users")
        user = request.form.get("username")
        for user_name in used_names:
            if user == user_name["username"]:
                return render_template("error.html", error="Username already in use")

        # Insert into database
        else:
            psw = generate_password_hash(request.form.get("password1"))
            db.execute("INSERT INTO users (username, hash) VALUES(:username, :password)", username=user, password=psw)
            return redirect("/login")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        return render_template("sell.html")

    else:
        # Check if user owns stock
        stock = request.form.get("stock")
        account_stock = db.execute("SELECT stock FROM shareholders WHERE user_id = :user_id AND stock = :stock_name",user_id = session["user_id"], stock_name = stock)
        if not account_stock:
            return render_template("error.html", error="User doesn't own any of this stock")

        # Check if user owns amount of shares
        amount = int(request.form.get("amount"))
        stock_amount = db.execute("SELECT amount FROM shareholders WHERE user_id = :user_id AND stock = :stock_name",user_id = session["user_id"], stock_name = stock)
        if stock_amount[0]["amount"] < amount:
            return render_template("error.html", error="User doesn't own as many of this stock")

        # Get total money from transaction
        stock_price = lookup(stock)
        total = stock_price["price"] * amount
        account_balance = db.execute("SELECT cash FROM users WHERE id=:number", number=session["user_id"])
        new_balance = account_balance[0]["cash"] + total

        # Update user account balance
        db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=new_balance, user_id=session["user_id"])

        # Remove stocks from user
        db.execute("UPDATE shareholders SET amount = :new_amount WHERE user_id = :user_id AND stock = :stock_name", new_amount = stock_amount[0]["amount"] - amount, user_id=session["user_id"], stock_name=stock)

        # Submit transaction to a history log
        db.execute("INSERT INTO history (user_id, stock, amount, total) VALUES(:user_id, :stock, :amount, :total)", user_id=session["user_id"], stock=stock, amount=-amount, total=total)

        # Return to index
        return redirect("/")

@app.route("/userdata", methods=["GET", "POST"])
def userdata():
    if request.method == "GET":
        return render_template("pswchange.html")
    else:
        old_password = generate_password_hash(request.form.get("old_password"))
        new_password = generate_password_hash(request.form.get("new_password"))

        # Ensure user provided correct old password
        old = db.execute("SELECT * FROM users WHERE id=:user_id", user_id=session["user_id"])
        if not check_password_hash(old[0]["hash"], request.form.get("old_password")):
            return render_template("error.html", error="Old password is incorrect")

        # Insert new password into database
        db.execute("UPDATE users SET hash=:new_password WHERE id=:user_id", new_password=new_password, user_id=session["user_id"])

        return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
