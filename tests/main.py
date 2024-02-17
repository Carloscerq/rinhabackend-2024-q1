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
    for i in range(1, 6):
        print('Teste ' + str(i))
        id = str(i)
        resp = make_transacao_request(id, 10, 'c', "Salario")
        if resp.status_code == 200:
            print('Transacao realizada com sucesso')
        else:
            print('Erro ao realizar transacao')

        resp = make_transacao_request(id, 10, 'd', "Aluguel")
        if resp.status_code == 200:
            print('Transacao realizada com sucesso')
        else:
            print('Erro ao realizar transacao')

        print("Make gigantic request")
        resp = make_transacao_request(id, 1000000, 'c', "Salario")

        print("See if it will fails the debitos request")
        resp = make_transacao_request(id, 1, "d", "Teste")
        if (resp.status_code != 422):
            print('Erro retornado corretamente')
        else:
            print('Erro retornado incorretamente')

        resp = make_extrato_request(id)
        if resp.status_code == 200:
            print('Extrato retornado com sucesso')

    resp = make_extrato_request('6')
    if resp.status_code == 404:
        print('Erro retornado corretamente')

if __name__ == '__main__':
    main()
