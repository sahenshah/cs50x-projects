import sqlite3

print("Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.")

# Connect to the SQLite database
conn = sqlite3.connect('fiftyville.db')
cursor = conn.cursor()

# Query 1: Get vehicle owners who exited the bakery on July 28, 2024, between 10:00 and 10:25
query1 = """
SELECT p.name AS vehicle_owner
FROM bakery_security_logs bsl
JOIN people p ON bsl.license_plate = p.license_plate
WHERE day = '28' AND month = '7' AND year = '2024' AND hour = '10' AND minute < '25' AND activity = 'exit';
"""
cursor.execute(query1)
vehicle_owners = [row[0] for row in cursor.fetchall()]

# Query 2: Get account holders who withdrew money from the Leggett Street ATM on July 28, 2024
query2 = """
SELECT p.name AS account_holder
FROM atm_transactions atm
JOIN bank_accounts ba ON atm.account_number = ba.account_number
JOIN people p ON ba.person_id = p.id
WHERE day = '28' AND month = '7' AND year = '2024' AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
"""
cursor.execute(query2)
account_holders = [row[0] for row in cursor.fetchall()]

# Query 3: Get caller and receiver names for phone calls less than 1 minute on July 28, 2024
query3 = """
SELECT caller.name AS caller_name, receiver.name AS receiver_name
FROM phone_calls pc
JOIN people caller ON pc.caller = caller.phone_number
JOIN people receiver ON pc.receiver = receiver.phone_number
WHERE day = '28' AND month = '7' AND year = '2024' AND duration < '60';
"""
cursor.execute(query3)
callers_receivers = cursor.fetchall()  # Fetch both caller and receiver names

# Query 4: Get flight id and destination city of the first flight out of Fiftyville on July 29, 2024
query4 = """
SELECT f.id, destination.city AS destination_city
FROM flights f
JOIN airports origin ON f.origin_airport_id = origin.id
JOIN airports destination ON f.destination_airport_id = destination.id
WHERE day = '29' AND month = '7' AND year = '2024' AND origin.city = 'Fiftyville'
ORDER BY hour, minute
LIMIT 1;
"""
cursor.execute(query4)
flight_info = cursor.fetchone()
flight_id = flight_info[0]
destination_city = flight_info[1]

# Query 5: Get list of passengers on the first flight out of Fiftyville
query5 = """
SELECT p.name AS passenger_name
FROM passengers pa
JOIN people p ON pa.passport_number = p.passport_number
WHERE pa.flight_id = ?;
"""
cursor.execute(query5, (flight_id,))
passengers = [row[0] for row in cursor.fetchall()]

# Close the database connection
conn.close()

# Compare the lists to find common names
common_names_q2 = list(set(vehicle_owners).intersection(set(account_holders)))
common_names_q3 = list(set(common_names_q2).intersection([row[0] for row in callers_receivers]))
common_names_q5 = list(set(common_names_q3).intersection(set(passengers)))

# Find the receiver name corresponding to the common name
receiver_name = None
for name in common_names_q5:
    for caller, receiver in callers_receivers:
        if caller == name:
            receiver_name = receiver
            break
    if receiver_name:
        break


# Check the number of common names
if len(common_names_q5) == 1:
    # Find the receiver name corresponding to the common name
    receiver_name = None
    for caller, receiver in callers_receivers:
        if caller == common_names_q5[0]:
            receiver_name = receiver
            break

    # Print the results
    print(f"The THIEF is: {common_names_q5[0]}")
    print(f"The city the thief ESCAPED TO: {destination_city}")
    print(f"The ACCOMPLICE is: {receiver_name}")
elif len(common_names_q5) > 1:
    print("Multiple suspects")
else:
    print("No suspects")


