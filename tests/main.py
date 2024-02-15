import requests
import os

def make_transacao_request(id: str, valor: int, tipo: chr, descricao: str):
    URL = os.getenv('BASE_URL') + '/clientes/' + id + '/transacoes'
    body = {
        "valor": valor,
        "tipo": tipo,
        "descricao": descricao
    }
    response = requests.post(URL, json=body)
    return response


def make_extrato_request(id: str):
    URL = os.getenv('BASE_URL') + '/clientes/' + id + '/extrato'
    response = requests.get(URL)
    return response


def main():
    print('Realizando testes sobre o usuario...')
    resp = make_transacao_request("1", 100000, 'c', "Salario")
    print(resp.json())

if __name__ == '__main__':
    main()
