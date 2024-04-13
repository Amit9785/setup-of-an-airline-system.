with open('output.txt', 'r') as file:
    lines = file.readlines()

    # Initialize a variable to keep track of whether a flight details section is being processed
    in_flight_details = False

    # Iterate over each line
    for line in lines:
        # Remove newline character from the end of the line
        line = line.strip()

        # Check if the line marks the start of flight details
        if line.startswith("Flight Details:"):
            # Print a separator before printing flight details
            print("-" * 50)
            # Mark that flight details section is being processed
            in_flight_details = True

        # Check if the line marks the end of flight details
        elif line == "":
            # Mark that flight details section is no longer being processed
            in_flight_details = False

        # If in flight details section, print the line
        elif in_flight_details:
            print(line)

    # Print a final separator after all flight details are printed
    print("-" * 50)