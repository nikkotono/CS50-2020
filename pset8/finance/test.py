from werkzeug.security import check_password_hash, generate_password_hash
from helpers import apology, login_required, lookup, usd
from datetime import datetime
from cs50 import SQL
db = SQL("sqlite:///finance.db")


# print(datetime.utcnow())

# stock = lookup('FB')

# stocks = []
# stocks = {'name': 'Facebook, Inc.', 'price': 175.19, 'symbol': 'FB'}
# stockz = stocks.items()
# for i in stockz:
#     print(i[1])
# dollar = 1.95
# usd1 = usd(dollar)
# print(stock)

# userRows = db.execute("SELECT * from users WHERE id = 4")
# print(userRows)

# for items in userRows:
#     username = items.get("username")
#     userId = items.get("id")
#     cash = items.get("cash")

# print(userId, username, cash)

transactions = db.execute("SELECT * FROM transactions WHERE user_id = 4")
sumQuotes = db.execute(
    'SELECT symbol, SUM(shares) as total_shares, SUM(cost) as total_cost FROM transactions WHERE user_id = 4 GROUP BY symbol'
    )
print(transactions[0])
print(transactions[1])
print(sumQuotes)
# for items in transactions:
#     userId    = items.get("user_id")
#     tr_symbol = items.get("symbol")
#     cost      = items.get("cost")
#     time      = items.get("time")
#     shares    = items.get("shares")

