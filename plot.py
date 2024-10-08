import pandas as pd
import matplotlib.pyplot as plt

# Read the output CSV file generated by the C++ program
data = pd.read_csv("soil_moisture_output.csv", parse_dates=['Date'])

# Ensure the 'Date' column is in datetime format
data['Date'] = pd.to_datetime(data['Date'], errors='coerce')

# Sort by date in case data is out of order
data = data.sort_values(by='Date')

# Calculate Moving Average (7-day window)
data['7-Day Moving Average'] = data['SoilMoisture'].rolling(window=7).mean()

# Create a figure with multiple subplots
plt.figure(figsize=(14, 10))

# Subplot 1: Time Series of Soil Moisture Levels
plt.subplot(3, 1, 1)
plt.plot(data['Date'], data['SoilMoisture'], label='Daily Soil Moisture', color='blue', marker='o', linestyle='-')
plt.title('Daily Soil Moisture Levels Over Time')
plt.xlabel('Date')
plt.ylabel('Soil Moisture (%)')
plt.legend()
plt.grid()

# Subplot 2: 7-Day Moving Average of Soil Moisture Levels
plt.subplot(3, 1, 2)
plt.plot(data['Date'], data['7-Day Moving Average'], label='7-Day Moving Average', color='orange')
plt.title('7-Day Moving Average of Soil Moisture Levels')
plt.xlabel('Date')
plt.ylabel('Soil Moisture (%)')
plt.legend()
plt.grid()

# Subplot 3: Drought and Flood Analysis
plt.subplot(3, 1, 3)
plt.plot(data['Date'], data['SoilMoisture'], label='Daily Soil Moisture', color='blue')
plt.fill_between(data['Date'], data['SoilMoisture'], where=(data['SoilMoisture'] < 20), color='red', alpha=0.3, label='Drought Condition (< 20%)')
plt.fill_between(data['Date'], data['SoilMoisture'], where=(data['SoilMoisture'] > 80), color='green', alpha=0.3, label='Flood Condition (> 80%)')
plt.title('Drought and Flood Analysis')
plt.xlabel('Date')
plt.ylabel('Soil Moisture (%)')
plt.legend()
plt.grid()

# Adjust layout and save the plot as a PNG file
plt.tight_layout()
plt.savefig("soil_moisture_analysis.png")  # Save the figure to a file
plt.show()  # Show the plot
