/*
Alunos:
  Jonath Wesley Herdt
  Lucas José da Cunha
*/
#include <WiFi.h>

//Definindo as credenciais da rede Wi-Fi
const char *ssid = "Unifique_WIFI_6387";
const char *password = "68866696";

// Define a porta do server como 80
WiFiServer server(80);

// Variavel para armazenar o HTTP request
String header;

// Retorna o tempo atual em milisegundos
unsigned long currentTime = millis();
// Retorna o tempo anterior
unsigned long previousTime = 0;
// Define o tempo maximo em milisegundos (exemplo: 2000ms = 2s)
const long timeoutTime = 2000;

// Variaveis para uso do sistema
int som = 1;

void setup() {
  Serial.begin(115200);
   // Conecta a rede Wi-Fi com o SSID e password
  Serial.print("Conectando a rede ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // Servidor disponivel para acesso
  WiFiClient client = server.available();
  // Caso haja alguma conexao
  if (client){ 
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("Cliente novo.");
    String currentLine = "";
    // Enquanto o cliente ficar conectado
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      // Caso haver algum dado para ler
      if (client.available()){ 
        char c = client.read();
        Serial.write(c);
        header += c;
        // caso o byte seja uma nova linha
        if (c == '\n') { 
          // Caso a linha atual esteja em branco, pega dois caracteres de nova linha de uma vez
          // esse é o fim do requeste do cliente HTTP, então envia uma resposta:
          if (currentLine.length() == 0) {
            // HTTP headers sempre começam o código de resposta (e.g. HTTP/1.1 200 OK)
            // e o content-type, entao o cliente sabe oque está recebendo, entao uma linha em branco:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Conexao: fechada");
            client.println();

            // Pagina Web em HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".buttonDefault { border: 2px solid #000000; border-radius: 50%; color: black; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println(".buttonQuantidade { border: 2px solid #000000; color: black; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }");
            client.println(".buttonAdd { text-decoration: none !important; }");
            client.println("</style></head>");
            client.println("<body><center><h1>PetFeeder</h1><h5>by Jonath & Lucas</h5></center><br>");
            client.println("<form id=\"myForm\"><center>");
            client.println("<h3>Racao</h3>");
            client.println("<div class=\"btn-group\">");
            client.println("<button class=\"buttonDefault\" name=\"power\" value=\"despejar\" type=\"submit\">Despejar</button>");
            client.println("</div><br>");
            client.println("<h3>Som</h3>");
            client.println("<div class=\"btn-group\">");
            if(som){
              client.println("<button class=\"buttonDefault\" name=\"som\" value=\"SomOff\" type=\"submit\">ON</button>");
            }else{
              client.println("<button class=\"buttonDefault\" name=\"som\" value=\"SomOn\" type=\"submit\">OFF</button>");
            }
            client.println("</div></form>");
            client.println("<h3>Quantidade</h3></center>");
            client.println("<div class=\"btn-group\">");
            client.println("<button class=\"buttonQuantidade\" name=\"quantidade\" value=\"100\" type=\"submit\">100 g</button>");
            client.println("<button class=\"buttonQuantidade\" name=\"quantidade\" value=\"200\" type=\"submit\">200 g</button>");
            client.println("<button class=\"buttonQuantidade\" name=\"quantidade\" value=\"300\" type=\"submit\">300 g</button>");
            client.println("<button class=\"buttonQuantidade\" name=\"quantidade\" value=\"400\" type=\"submit\">400 g</button>");
            client.println("<button class=\"buttonQuantidade\" name=\"quantidade\" value=\"500\" type=\"submit\">500 g</button><br></div>");
            client.println("<h3>Horarios</h3>");
            client.println("<div class=\"horariosContainer\" id=\"horariosContainer\">");
            client.println("<input type=\"time\" value=\"\" name=\"horario0\" id=\"horario0\"/><a class=\"buttonAdd\" href=\"javascript:void(0)\" onclick=\"addHorario(0)\" id=\"addLink0\"> +</a>");
            client.println("</div><br>");
            client.println("<script>");
            client.println("function addHorario(id){");
            client.println("if(id < 4){");
            client.println("var horarios = document.getElementById('horariosContainer');");
            client.println("var el = 'addLink' + id;");
            client.println("var addLink = document.getElementById(el);");
            client.println("addLink.style.display = 'none';");
            client.println("if(id > 0){");
            client.println("el = 'deleteLink' + id;");
            client.println("var deleteLink = document.getElementById(el);");
            client.println("deleteLink.style.display = 'none';}");
            client.println("id += 1;");
            client.println("var br = document.createElement('br');");
            client.println("br.id = 'br' + id;");
            client.println("horarios.appendChild(br);");
            client.println("var input = document.createElement('input');");
            client.println("input.id = 'horario' + id;");
            client.println("input.type = 'time';");
            client.println("input.name = 'horario' + id;");
            client.println("horarios.appendChild(input);");
            client.println("if(id < 4){");
            client.println("var linkAdd = document.createElement('a');");
            client.println("linkAdd.id = 'addLink' + id;");
            client.println("linkAdd.text = ' +';");
            client.println("linkAdd.href = 'javascript:void(0)';");
            client.println("linkAdd.setAttribute('class', 'buttonAdd');");
            client.println("linkAdd.setAttribute('onclick', 'addHorario('+id+')');");
            client.println("horarios.appendChild(linkAdd);}");
            client.println("var linkDelete = document.createElement('a');");
            client.println("linkDelete.id = 'deleteLink' + id;");
            client.println("linkDelete.text = ' -';");
            client.println("linkDelete.href = 'javascript:void(0)';");
            client.println("linkDelete.setAttribute('class', 'buttonAdd');");
            client.println("linkDelete.setAttribute('onclick', 'removeElement('+id+')');");
            client.println("horarios.appendChild(linkDelete);}}");
            client.println("function removeElement(id){");
            client.println("var elem = document.getElementById('horario'+id);");
            client.println("elem.parentNode.removeChild(elem);");
            client.println("if(id < 4){");
            client.println("elem = document.getElementById('addLink'+id);");
            client.println("elem.parentNode.removeChild(elem);}");
            client.println("elem = document.getElementById('deleteLink'+id);");
            client.println("elem.parentNode.removeChild(elem);");
            client.println("elem = document.getElementById('br'+id);");
            client.println("elem.parentNode.removeChild(elem);");
            client.println("id -= 1;");
            client.println("var el = 'addLink' + id;");
            client.println("var addLink = document.getElementById(el);");
            client.println("addLink.style.display = '';");
            client.println("if(id > 0){");
            client.println("el = 'deleteLink' + id;");
            client.println("var deleteLink = document.getElementById(el);");
            client.println("deleteLink.style.display = '';}}");
            client.println("</script></body></html>");
            break;
          }
          else { // caso ter uma nova linha, limpa a linha atual
            currentLine = "";
          }
        } else if (c != '\r') { // se possuir algum outro dado, retorna o caracter
          currentLine += c; // adiciona ele ao final da linha atual
        }
      }
    }
    // Limpa o cabecalho
    header = "";
    // Encerra a conexao
    client.stop();
    Serial.println("Cliente desconectado.");
    Serial.println("");
  }

  delay(10);
}