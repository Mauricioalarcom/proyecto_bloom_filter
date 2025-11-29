import random
import string

def generate_url(is_malicious=False):
    tlds = ['.com', '.net', '.org', '.io', '.xyz', '.info']
    malicious_words = ['phishing', 'login', 'secure', 'account', 'update', 'verify', 'bank']
    safe_words = ['google', 'facebook', 'youtube', 'amazon', 'wikipedia', 'reddit']
    
    if is_malicious:
        base = random.choice(malicious_words)
        suffix = ''.join(random.choices(string.ascii_lowercase + string.digits, k=5))
        return f"http://{base}-{suffix}{random.choice(tlds)}"
    else:
        base = random.choice(safe_words)
        suffix = ''.join(random.choices(string.ascii_lowercase + string.digits, k=5))
        return f"https://www.{base}{suffix}{random.choice(tlds)}"

def generate_file(filename, count, malicious_ratio=0.5):
    with open(filename, 'w') as f:
        for _ in range(count):
            is_malicious = random.random() < malicious_ratio
            url = generate_url(is_malicious)
            f.write(url + '\n')

if __name__ == "__main__":
    # Generate blacklist (all malicious for simplicity in this specific test setup, 
    # or a mix, but usually blacklist contains only bad stuff)
    print("Generating blacklist_urls.txt...")
    with open('tests/blacklist_urls.txt', 'w') as f:
        for _ in range(50000):
            f.write(generate_url(is_malicious=True) + '\n')
            
    # Generate queries (mix of known malicious, new malicious, and safe)
    print("Generating test_queries.txt...")
    with open('tests/test_queries.txt', 'w') as f:
        # 10k queries
        for _ in range(10000):
            # 20% chance of being a known malicious URL (we'd need to pick from the list, 
            # but for simplicity let's just generate random ones here. 
            # To properly test True Positives, we should ideally reuse some.
            # However, the prompt asks for "Consultar muchas URLs que NO están" for False Positives.
            # Let's generate mostly new URLs to test FP and TN.
            f.write(generate_url(is_malicious=random.random() < 0.5) + '\n')
            
    print("Done.")
