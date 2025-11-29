import random
import string

def generate_url(is_malicious=False):
    tlds = ['.com', '.net', '.org', '.io', '.xyz', '.info']
    # Palabras que suelen usarse en URLs de phishing
    malicious_words = ['phishing', 'login', 'secure', 'account', 'update', 'verify', 'bank', 'signin', 'wallet']
    # Dominios legítimos conocidos
    safe_domains = ['google', 'facebook', 'youtube', 'amazon', 'wikipedia', 'reddit', 'twitter', 'instagram', 'linkedin']
    
    if is_malicious:
        # Generar algo que parezca phishing, ej: http://secure-login-amazon-update.xyz
        base = random.choice(malicious_words)
        target = random.choice(safe_domains) # A menudo el phishing imita marcas reales
        suffix = ''.join(random.choices(string.ascii_lowercase + string.digits, k=4))
        # Estructura típica de phishing: http, guiones, palabras de urgencia
        return f"http://{base}-{target}-{suffix}{random.choice(tlds)}"
    else:
        # Generar una URL legítima, ej: https://www.amazon.com/product/abcde
        domain = random.choice(safe_domains)
        tld = random.choice(['.com', '.org', '.net']) # TLDs más comunes para sitios seguros
        path = ''.join(random.choices(string.ascii_lowercase + string.digits, k=8))
        return f"https://www.{domain}{tld}/{path}"

def generate_file(filename, count, malicious_ratio=0.5):
    with open(filename, 'w') as f:
        for _ in range(count):
            is_malicious = random.random() < malicious_ratio
            url = generate_url(is_malicious)
            f.write(url + '\n')

if __name__ == "__main__":
    # Generar blacklist (solo URLs maliciosas para la base de datos de virus)
    print("Generando blacklist_urls.txt (50k URLs maliciosas)...")
    with open('tests/blacklist_urls.txt', 'w') as f:
        for _ in range(50000):
            f.write(generate_url(is_malicious=True) + '\n')
            
    # Generar queries (mezcla de tráfico normal y ataques)
    print("Generando test_queries.txt (10k consultas)...")
    with open('tests/test_queries.txt', 'w') as f:
        for _ in range(10000):
            # 50% de probabilidad de ser una URL maliciosa (ataque)
            # 50% de probabilidad de ser una URL segura (tráfico normal)
            # Nota: Las maliciosas generadas aquí serán "nuevas" (no estarán en la blacklist exacta)
            # a menos que tengamos suerte, lo cual es bueno para probar Falsos Positivos del filtro
            # con URLs que se PARECEN a las malas pero no son idénticas, 
            # o simplemente URLs malas nuevas que el filtro NO detectará (Falsos Negativos del sistema, 
            # aunque el Bloom Filter solo garantiza no Falsos Negativos si la URL ESTÁ en el set).
            
            # Para probar Falsos Positivos del Bloom Filter (decir que SI está cuando NO está),
            # necesitamos URLs que NO estén en la blacklist.
            # Tanto las seguras como las nuevas maliciosas sirven para esto.
            f.write(generate_url(is_malicious=random.random() < 0.5) + '\n')
            
    print("Datos generados correctamente.")
