#include "shell.hh"

int main(){ 
    std::string input;
    std::string prompt;
    int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755); // fd = file descriptor, 0755 = juiste rechten geven (leesbaar).
    char byte[1]; // variabele byte, grootte van 1 byte, datatype char.
    while(syscall(SYS_read, fd, byte, 1)){ // Blijft SYS_read loopen totdat het bestand compleet gelezen is.
      prompt.append(byte); // append de byte aan het prompt.txt bestand.
  }


  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file() // ToDo: Implementeer volgens specificatie.
{ 
    std::string invoertekst;
    std::string bestandsnaam;
    std::string line_str;

    std::cout << "Geef uw bestandsnaam op:" << std::endl;
    std::getline(std::cin, bestandsnaam);
    const char* bestand[] = {bestandsnaam.c_str()};
    int fd = syscall(SYS_creat, bestand[0], 0755);

    std::cout << "Geef uw tekst op. (typ EOF om het programma te sluiten)" << std::endl;
    while(std::getline(std::cin, invoertekst)){
        if(invoertekst == "<EOF>"){
            break;
        }
        line_str += invoertekst + '\n';
    }
    const char* line[] = {line_str.c_str()};
    syscall(SYS_write, fd, line[0], line_str.size());
   
}
void list() // ToDo: Implementeer volgens specificatie.
{ 
    if (syscall(SYS_fork) == 0){
        const char* flags[] = {"/bin/ls", "-l", "-a", NULL};
        syscall(SYS_execve, flags[0], flags, NULL);
    }
    else{
        syscall(SYS_wait4, 0, NULL, NULL, NULL);
    }
 }

void find() // ToDo: Implementeer volgens specificatie.
{   std::cout << "Waarop wilt u gaan zoeken?" << std::endl;
    std::string input = "";
    std::getline(std::cin, input);
    int fd[2];
    syscall(SYS_pipe, &fd);
    if(syscall(SYS_fork) == 0){
        syscall(SYS_close, fd[0]);
        syscall(SYS_dup2, fd[1], 1);
        const char* flags[] = {"/usr/bin/find", ".", NULL};
        syscall(SYS_execve, flags[0], flags, NULL);
    }
    else{
        syscall(SYS_wait4, 0, NULL, NULL, NULL);
    }
    if(syscall(SYS_fork) == 0){
        syscall(SYS_close, fd[1]);
        syscall(SYS_dup2, fd[0], 0);
        const char* flags2[] = {"/bin/grep", input.c_str(), NULL};
        syscall(SYS_execve, flags2[0], flags2, NULL);
    }
    else{
        syscall(SYS_close, fd[1]);
        syscall(SYS_close, fd[0]);
        syscall(SYS_wait4, 0, NULL, NULL);
    }
}

void seek() // ToDo: Implementeer volgens specificatie.
{ 
    int seek = syscall(SYS_creat, "seek", 0755);
    int loop = syscall(SYS_creat, "loop", 0755);
    char byte1[1] = {'x'};
    char byte2[1] = {'\0'};
    int mil5 = 5000000;

    syscall(SYS_write, seek, byte1, 1);
    syscall(SYS_lseek, seek, mil5, 1);
    syscall(SYS_write, seek, byte1, 1);
    syscall(SYS_write, loop, byte1, 1);
    
    for(int i=0; i < mil5; i++){
        syscall(SYS_write, loop, byte2, 1);
    }
    syscall(SYS_write, loop, byte1, 1); 

}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
