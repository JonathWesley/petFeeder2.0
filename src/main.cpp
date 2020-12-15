/*
Alunos:
  Jonath Wesley Herdt
  Lucas José da Cunha
*/
#include <WiFi.h>
#include <NTPClient.h>

//Definindo as credenciais da rede Wi-Fi
const char *ssid = "Unifique_WIFI_6387";
const char *password = "68866696";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

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

// Variaveis de auxilio
int posicaoQuantidade;
String Squantidade = "100", Sdespejar = "of";

// Variaveis para uso do sistema
bool som = false, despejar = false;
int quantidade = 100, quantidadeHorarios = 1;
String horarios[5];

void despejarRacao();

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

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(-10800);
}

void loop() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  //Serial.println(formattedDate);
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  //Serial.print("DATE: ");
  //Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  //Serial.print("HOUR: ");
  //Serial.println(timeStamp);

  if(despejar){
    despejarRacao();
  }

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

            if(header.indexOf("quantidade") != -1){
              posicaoQuantidade = header.indexOf("quantidade") + 11;
              Squantidade = header.substring(posicaoQuantidade, posicaoQuantidade + 3);
              quantidade = Squantidade.toInt();
            }

            if(header.indexOf("despejar") != -1){
              posicaoQuantidade = header.indexOf("despejar") + 9;
              Sdespejar = header.substring(posicaoQuantidade, posicaoQuantidade + 2);
              if(Sdespejar == "on"){
                despejar = true;
                header[posicaoQuantidade + 1] = 'f';
              }
            }

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
            client.println("<button class=\"buttonDefault\" name=\"despejar\" value=\"on\" type=\"submit\">Despejar</button>");
            client.println("</div><br>");
            client.println("<h3>Som</h3>");
            client.println("<div class=\"btn-group\">");
            if(som){
              client.println("<button class=\"buttonDefault\" name=\"som\" value=\"SomOff\" type=\"submit\">OFF</button>");
            }else{
              client.println("<button class=\"buttonDefault\" name=\"som\" value=\"SomOn\" type=\"submit\">ON</button>");
            }
            client.println("</div>");
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
            client.println("</div><br></form>");
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

void despejarRacao(){
  Serial.println("Despejando racao!");
  despejar = false;
}