# scc

Please donate if you find this program useful

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](http://paypal.me/DemetryRom)

**Please note this program is free for open source use commercially or non commercially as long as its open source
  for closed source please email demetryromanowski@gmail.com for licensing**
  
  

Scc is a SQL Composer/Compiler allowing the user to create a database project and split it into many files
  The jist as follows 
  - Create a directory structure
  - Create your sql files
  - Magic
  - Outputs your compiled file to run server side 

### New Features!

  - Use **script** and **endscript** to put quick scripts in between **includes**
  
### Known bugs
  - Random crashes with some directory names

## How to use scc in an ide like DataGrip

**Step 1:**
1. Go ahead and create yourself a new project
![alt text](https://i.gyazo.com/3093d004c6379ebae686f1e3786fcded.png "Create a new project")
**In DataGrip it will ask you if you would like to open in This Window or New Window just pick one that is suitable for you**

2. Now go ahead an press **Alt+1** and **Alt+2** if the Database view and File view isn't already opened
![alt text](https://i.gyazo.com/f50c4fd86520c293d62fd7d40d835bf9.png "")
 **Your Datagrip should look like mine above**

3. Add your Database Source with the + on the left side and fill in your info.  
  **I don't have a database set up so I will skip this step**

4. Now you have to attach your project directory by right clicking in the files view tab
![alt text](https://i.gyazo.com/6063f1734e09597cca9e330940f804de.png "")
**You can attach any directory that you want, best to create a new one for your project**

5. Create your directory structure
![alt text](https://i.gyazo.com/2254b1349b46607b8af82df8d9d086a6.png "")

- Notice the scc folder holds scc.exe this is best practice for now because in auto build mode (no command line args) 
  it will look for makesqlc or make.sqlc in its current working directory

- Now I like to name and build the directory structure as seen with src being the topmost directory
  you can name it and organize it however you want because scc doesn't look for the files you define the includes in       **makesqlc** or **make.sqlc**

6. Create your project sources eg:
![alt text](https://i.gyazo.com/9ef601b06d32cbcab9ffa8b2858d29ca.png "")
- I create each function and table in its own file making it easier to manage than one large file

    - This will be a brief description of the makesqlc language although not many features are currently available
      it is powerful enough for most projects as it currently stands
        ```SQL
        --This is a comment
        --Multiline comments are a planned feature
        --include can take wildcard or specific paths eg:
        include ../src/functions/void/CleanUpDB.sql
        include ../src/functions/void/*.sql
        
        --order of operations is very important
        --the first include will be compiled into the output at the top and get put in descending order
        --eg
        include 1.sql
        include 2.sql
        include 3.sql
        
        --output:
            1.sql contents
        
            2.sql contents
        
            3.sql contents
        
        include ../src/functions/get/*.sql
        include ../src/functions/set/*.sql
    
        --Now as you can see the line below will run everything including cleanup db
        include ../src/functions/void/*.sql
        --An ignore keyword is coming but for now the way to do this is to move 
        --CleanUpDB.sql into its own directory and include it as needed or get 
        --rid of it and paste the contents
        --between the script / endscript tags as such below
        
        --This is the script keyword notice the endscript this is essential because 
        --if you don't have it scc will output everything after script keyword 
        --as text in the output.sql
        
        script
        
        -- quick script contents
        DROP SCHEMA public CASCADE;
        CREATE SCHEMA public;
        GRANT ALL ON SCHEMA public TO postgres;
        GRANT ALL ON SCHEMA public TO public;
        COMMENT ON SCHEMA public IS 'standard public schema';
        
        endscript
        
        include ../src/tables/*.sql
        ```
        
     - So to conclude in sqlc you have 
        1. ```-- comments```
        2. ```includes```
        3. ```script```
        4. ```endscript```
    
7. Now create your makesqlc or make.sqlc mine would look like this
    ```SQL
    include ../src/functions/void/CleanUpDB.sql

    include ../src/tables/*.sql
    
    include ../src/functions/get/*.sql
    include ../src/functions/set/*.sql
    
    script
    
    PERFORM AddUserPermissions() 
    
    endscript

    ```

8. Run scc in automode 
  - If you have done everthing correctly a output.sql will apear with the following contents in my case
      ```
      -- Drops the table and the routines.
        DROP SCHEMA public CASCADE;
        CREATE SCHEMA public;
        GRANT ALL ON SCHEMA public TO postgres;
        GRANT ALL ON SCHEMA public TO public;
        COMMENT ON SCHEMA public IS 'standard public schema';
        
        CREATE TABLE "UserNames" (
        	"userID" serial NOT NULL,
        	"username" varchar NOT NULL UNIQUE,
        	"firstName" varchar NOT NULL,
        	"lastName" varchar NOT NULL,
        	"email" varchar,
        	"phoneNumber" varchar,
        	CONSTRAINT Users_pk PRIMARY KEY ("userID")
        ) WITH (
          OIDS=FALSE
        );
        
        CREATE OR REPLACE FUNCTION "GetUsernames"() RETURNS TABLE()
        	LANGUAGE plpgsql
        AS $$
        DECLARE
        
          BEGIN
            RETURN QUERY
              SELECT "username"
              FROM "Usernames"
              WHERE active = TRUE;
          END;
        $$
        ;
        
        CREATE OR REPLACE FUNCTION "AddUsername"(_name character varying) RETURNS integer
        	LANGUAGE plpgsql
        AS $$
        DECLARE
        
            lastid INT;
        
          BEGIN
        
        
              INSERT INTO "Usernames" ("name")
              VALUES (_name) RETURNING "userID" INTO lastid;
        
        
            RETURN lastid;
        
          END;
        $$
        ;
        
        PERFORM AddUserPermissions()
      ```

    #### Now as you can see it composed all the files in order as specifed by makesqlc 
    
9. Now you can run this output.sql in your database and it will generate and set up your database correctly
    
# Planned features
1. Syntax correction and code error checking in the output
2. More features in the sqlc language
    - ignore 
    - multine comments
    - feature requests ? 
3. A plugin for DataGrip to allow for project creation and compiling without running scc externally
4. feature requests ? 

#### To request a feature email me @ demetryromanowski@gmail.com


