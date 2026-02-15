import requests

def predict_gender(name):
    url = f"https://api.genderize.io?name={name}"
    response = requests.get(url)
    data = response.json()

    if 'gender' in data and data['gender']:
        gender = data['gender']
        probability = data['probability']
        print(f"The name '{name}' is likely {gender} (confidence: {float(probability)*100:.1f}%)")
    else:
        print(f"The name '{name}' could not be classified. It might be unisex or unknown.")

# Example
user_input = input("Enter a name: ")
predict_gender(user_input)
