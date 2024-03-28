#include"shaderClass.h"

// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const std::string& filepath){
	std::ifstream file(filepath); // ファイルを開く

    if (!file.is_open()) {
        std::cerr << "cannot open file path: " << filepath << std::endl;
        std::exit(1); 
    }
    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())); // ファイル内容を文字列として読み込む

    file.close(); // ファイルを閉じる
	return content;
}

// シェーダオブジェクトのコンパイル結果を表示する
// shader: シェーダオブジェクト名
// str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
	// コンパイル結果を取得する
	
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
	// シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
	if (bufSize > 1)
	{
	// シェーダのコンパイル時のログの内容を取得する
	std::vector<GLchar> infoLog(bufSize);
	GLsizei length;
	glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
	std::cerr << &infoLog[0] << std::endl;
	}
 return static_cast<GLboolean>(status);
}

// プログラムオブジェクトのリンク結果を表示する
// program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program){
	// リンク結果を取得する
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
	// シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
	if (bufSize > 1)
	{
	// シェーダのリンク時のログの内容を取得する
	std::vector<GLchar> infoLog(bufSize);
	GLsizei length;
	glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
	std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile){

	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	ID = glCreateProgram();// Create Shader Program Object and get its reference

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);// Create Vertex Shader Object and get its reference
	glShaderSource(vertexShader, 1, &vertexSource, NULL);// Attach Vertex Shader source to the Vertex Shader Object
	glCompileShader(vertexShader);// Compile the Vertex Shader into machine code
	if (printShaderInfoLog(vertexShader, "vertex shader")){
		glAttachShader(ID, vertexShader);
	}
	glDeleteShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);// Create Fragment Shader Object and get its reference
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);// Attach Fragment Shader source to the Fragment Shader Object
	glCompileShader(fragmentShader);// Compile the Vertex Shader into machine code
	if (printShaderInfoLog(fragmentShader, "fragment shader")){
		glAttachShader(ID, fragmentShader);	
	}
	glDeleteShader(fragmentShader);
	
	glLinkProgram(ID);// Wrap-up/Link all the shaders together into the Shader Program
	glBindAttribLocation(ID, 0, "position");
	glBindFragDataLocation(ID, 0, "fragment");

	// プログラムオブジェクトが作成できなければ 0 を返す
	if (!printProgramInfoLog(ID))this->Delete();	
}

// Activates the Shader Program
void Shader::Activate(){
	glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}