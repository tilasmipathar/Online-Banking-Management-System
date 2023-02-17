#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct normalUser
{
    int userID;
    char name[30];
    char password[10];
    int account_no;
    float balance;
    char status[20];
} normalUser;

typedef struct jointUser
{
    int userID;
    char name1[30];
    char name2[30];
    char password[10];
    int account_no;
    float balance;
    char status[20];
} jointUser;

typedef struct admin
{
    int userID;
    char username[30];
    char password[10];
} admin;

normalUser getNormalUser(int ID)
{
    int i = ID - 1000;
    normalUser currUser;
    int fd = open("NormalUserFile", O_RDONLY, 0744);

    int fl1;
    struct flock lock;
    
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); 
    lock.l_len = sizeof(normalUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
    read(fd, &currUser, sizeof(normalUser));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

jointUser getJointUser(int ID)
{
    int i = ID - 1000;
    jointUser currUser;
    int fd = open("JointUserFile", O_RDONLY, 0744);

    int fl1;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser); 
    lock.l_len = sizeof(jointUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &currUser, sizeof(jointUser));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

admin getAdmin(int ID)
{
    int i = ID - 1000;
    admin currUser;
    int fd = open("AdminFile", O_RDONLY, 0744);
    int fl1;
    
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(admin); 
    lock.l_len = sizeof(admin);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    lseek(fd, (i) * sizeof(admin), SEEK_SET);
    read(fd, &currUser, sizeof(admin));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return currUser;
}

bool checkNormalUser(normalUser currUser)
{
    int i = currUser.userID - 1000;
    int fd = open("NormalUserFile", O_RDONLY, 0744);
    bool rslt;
    normalUser temp;

    int fl1;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); 
    lock.l_len = sizeof(normalUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 
    
    lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
    read(fd, &temp, sizeof(normalUser));

    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
    {
        rslt = true;
    }

    else
    {
        rslt = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool checkJointUser(jointUser currUser)
{
    int i = currUser.userID - 1000;
    int fd = open("JointUserFile", O_RDONLY, 0744);
    bool rslt;
    jointUser temp;

    int fl1;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser); 
    lock.l_len = sizeof(jointUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &temp, sizeof(jointUser));

    if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
    {
        rslt = true;
    }

    else
    {
        rslt = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool checkAdmin(admin currUser)
{
    int i = currUser.userID - 1000;
    int fd = open("AdminFile", O_RDONLY, 0744);
    bool rslt;
    admin temp;

    int fl1;
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(admin); 
    lock.l_len = sizeof(admin);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    lseek(fd, (i) * sizeof(admin), SEEK_SET); 
    read(fd, &temp, sizeof(admin));

    if (!strcmp(temp.password, currUser.password))
    { 
        rslt = true;
    }

    else
    {
        rslt = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool depositMoney(int accType, int ID, float amt)
{
    int i = ID - 1000;
    
    if (accType == 1)
    {
        int fd = open("NormalUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;
        
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); 
        lock.l_len = sizeof(normalUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock); 

        normalUser currUser;

        lseek(fd, (i) * sizeof(normalUser), SEEK_SET);
        read(fd, &currUser, sizeof(normalUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amt;
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    else if (accType == 2)
    {
        int fd = open("JointUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;

        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser); 
        lock.l_len = sizeof(jointUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock); 

        jointUser currUser;

        lseek(fd, (i) * sizeof(jointUser), SEEK_SET); 
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            currUser.balance += amt;
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    return false;
}

bool withdrawMoney(int accType, int ID, float amt)
{

    int i = ID - 1000;
    
    if (accType == 1)
    {
        int fd = open("NormalUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;
    
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); 
        lock.l_len = sizeof(normalUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        normalUser currUser;

        lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
        read(fd, &currUser, sizeof(normalUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amt)
        {
            currUser.balance -= amt;
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    else if (accType == 2)
    {
        int fd = open("JointUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;
    
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser); 
        lock.l_len = sizeof(jointUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock); 

        jointUser currUser;

        lseek(fd, (i) * sizeof(jointUser), SEEK_SET); 
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE") && currUser.balance >= amt)
        {
            currUser.balance -= amt;
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    return false;
}

float getBalance(int accType, int ID)
{
    int i = ID - 1000;
    float rslt;
    if (accType == 1)
    {
        int i = ID - 1000;
        int fd = open("NormalUserFile", O_RDONLY, 0744);
        normalUser temp;

        int fl1;
        struct flock lock;
    
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); 
        lock.l_len = sizeof(normalUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock); 

        lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
        read(fd, &temp, sizeof(normalUser));

        if (!strcmp(temp.status, "ACTIVE"))
        {
            rslt = temp.balance;
        }
        
        else
        {
            rslt = 0;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    else if (accType == 2)
    {
        int i = ID - 1000;
        int fd = open("JointUserFile", O_RDONLY, 0744);
        jointUser temp;

        int fl1;
        struct flock lock;
    
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser); 
        lock.l_len = sizeof(jointUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(jointUser), SEEK_SET); 
        read(fd, &temp, sizeof(jointUser));

        if (!strcmp(temp.status, "ACTIVE"))
        {
            rslt = temp.balance;
        }

        else
        {
            rslt = 0;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    return 0;
}

bool alterPassword(int accType, int ID, char newPwd[10])
{
    int i = ID - 1000;

    if (accType == 1)
    {
        int fd = open("NormalUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;
     
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(normalUser); 
        lock.l_len = sizeof(normalUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock); 

        normalUser currUser;

        lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
        read(fd, &currUser, sizeof(normalUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPwd);
            lseek(fd, sizeof(normalUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(normalUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    else if (accType == 2)
    {
        int fd = open("JointUserFile", O_RDWR, 0744);
        bool rslt;
        int fl1;
        struct flock lock;
    
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(jointUser); 
        lock.l_len = sizeof(jointUser);         
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        jointUser currUser;

        lseek(fd, (i) * sizeof(jointUser), SEEK_SET); 
        read(fd, &currUser, sizeof(jointUser));

        if (!strcmp(currUser.status, "ACTIVE"))
        {
            strcpy(currUser.password, newPwd);
            lseek(fd, sizeof(jointUser) * (-1), SEEK_CUR);
            write(fd, &currUser, sizeof(jointUser));
            rslt = true;
        }

        else
        {
            rslt = false;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        
        return rslt;
    }

    return false;
}

bool addNormalUser(normalUser record)
{
    int fd = open("NormalUserFile", O_RDWR, 0744);
    bool rslt;

    int fl1;
    struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1) * sizeof(normalUser); 
    lock.l_len = sizeof(normalUser);          
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    normalUser endUser;

    lseek(fd, (-1) * sizeof(normalUser), SEEK_END); 
    read(fd, &endUser, sizeof(normalUser));

    record.userID = endUser.userID + 1;
    record.account_no = endUser.account_no + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(normalUser));

    if (j != 0)
    {
        rslt = true;
    }

    else
    {
        rslt = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool addJointUser(jointUser record)
{
    int fd = open("JointUserFile", O_RDWR, 0744);
    bool rslt;

    int fl1;
    struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start = (-1) * sizeof(jointUser); 
    lock.l_len = sizeof(jointUser);          
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    jointUser endUser;

    lseek(fd, (-1) * sizeof(jointUser), SEEK_END); 
    read(fd, &endUser, sizeof(jointUser));

    record.userID = endUser.userID + 1;
    record.account_no = endUser.account_no + 1;
    strcpy(record.status, "ACTIVE");

    int j = write(fd, &record, sizeof(jointUser));

    if (j != 0)
    {
        rslt = true;
    }

    else
    {
        rslt = false;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool deleteNormalUser(int ID)
{
    int i = ID - 1000;
    int fd = open("NormalUserFile", O_RDWR, 0744);
    bool rslt;

    int fl1;
    struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); 
    lock.l_len = sizeof(normalUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    normalUser currUser;

    lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
    read(fd, &currUser, sizeof(normalUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED");
        currUser.balance = 0;

        lseek(fd, (-1) * sizeof(normalUser), SEEK_CUR);

        int j = write(fd, &currUser, sizeof(normalUser));
        if (j != 0)
        {
            rslt = true;
        }

        else
        {
            rslt = false;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool deleteJointUser(int ID)
{
    int i = ID - 1000;
    int fd = open("JointUserFile", O_RDWR, 0744);
    bool rslt;

    int fl1;
    struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser); 
    lock.l_len = sizeof(jointUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    jointUser currUser;

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET);
    read(fd, &currUser, sizeof(jointUser));

    if (!strcmp(currUser.status, "ACTIVE"))
    {
        strcpy(currUser.status, "CLOSED");
        currUser.balance = 0;

        lseek(fd, (-1) * sizeof(jointUser), SEEK_CUR);

        int j = write(fd, &currUser, sizeof(jointUser));
        if (j != 0)
        {
            rslt = true;
        }

        else
        {
            rslt = false;
        }

    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool modifyNormalUser(normalUser modUser)
{
    int i = modUser.userID - 1000;
    int fd = open("NormalUserFile", O_RDWR, 0744);
    bool rslt = false;

    int fl1;
    struct flock lock;
 
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(normalUser); 
    lock.l_len = sizeof(normalUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    normalUser currUser;

    lseek(fd, (i) * sizeof(normalUser), SEEK_SET); 
    read(fd, &currUser, sizeof(normalUser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.account_no == currUser.account_no))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1) * sizeof(normalUser), SEEK_CUR);

        int j = write(fd, &modUser, sizeof(normalUser));
        if (j != 0)
        {
            rslt = true;
        }

        else
        {
            rslt = false;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

bool modifyJointUser(jointUser modUser)
{
    int i = modUser.userID - 1000;
    int fd = open("JointUserFile", O_RDWR, 0744);
    bool rslt = false;

    int fl1;
    struct flock lock;
    
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(jointUser); 
    lock.l_len = sizeof(jointUser);         
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock); 

    jointUser currUser;

    lseek(fd, (i) * sizeof(jointUser), SEEK_SET); 
    read(fd, &currUser, sizeof(jointUser));

    if (!strcmp(currUser.status, "ACTIVE") && (modUser.account_no == currUser.account_no))
    {
        strcpy(modUser.status, "ACTIVE");
        lseek(fd, (-1) * sizeof(jointUser), SEEK_CUR);

        int j = write(fd, &modUser, sizeof(jointUser));
        if (j != 0)
        {
            rslt = true;
        }

        else
        {
            rslt = false;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    
    return rslt;
}

void serverTask(int nsd)
{
    int msgLength, select, type, option, accType, currUserID;
    bool rslt;
    
    while (1)
    {
        read(nsd, &option, sizeof(option));
        printf("Option : %d\n", option);

        if (option == 1)
        {
            normalUser currUser1;
    
            accType = 1;
            msgLength = read(nsd, &currUser1, sizeof(normalUser));
    
            printf("Username : %d\n", currUser1.userID);
            printf("Password : %s\n", currUser1.password);
    
            currUserID = currUser1.userID;
            rslt = checkNormalUser(currUser1);
    
            write(nsd, &rslt, sizeof(rslt));
        }
    
        else if (option == 2)
        {
            jointUser currUser2;
    
            accType = 2;
            msgLength = read(nsd, &currUser2, sizeof(jointUser));
            currUserID = currUser2.userID;
    
            printf("Username : %d\n", currUser2.userID);
            printf("Password : %s\n", currUser2.password);
    
            rslt = checkJointUser(currUser2);
    
            write(nsd, &rslt, sizeof(rslt));
        }
    
        else if (option == 3)
        {
            admin currUser3;
    
            accType = 3;
            msgLength = read(nsd, &currUser3, sizeof(admin));
            currUserID = currUser3.userID;
    
            printf("Username : %d\n", currUser3.userID);
            printf("Password : %s\n", currUser3.password);
    
            rslt = checkAdmin(currUser3);
    
            write(nsd, &rslt, sizeof(rslt));
        }
    
        else
        {
            rslt = false;
            write(nsd, &rslt, sizeof(rslt));
        }
    
        if (rslt)
        {
            break;
        }
    }

    while (1)
    {
        read(nsd, &select, sizeof(int));

        if (option == 1 || option == 2)
        {
            if (select == 1)
            {
                float amt;

                read(nsd, &amt, sizeof(float));
                rslt = depositMoney(accType, currUserID, amt);
                write(nsd, &rslt, sizeof(rslt));
            }

            else if (select == 2)
            {
                float amt;

                read(nsd, &amt, sizeof(float));
                rslt = withdrawMoney(accType, currUserID, amt);
                write(nsd, &rslt, sizeof(rslt));
            }

            else if (select == 3)
            {
                float amt;

                amt = getBalance(accType, currUserID);
                write(nsd, &amt, sizeof(float));
            }

            else if (select == 4)
            {
                char pwd[10];

                read(nsd, pwd, sizeof(pwd));
                rslt = alterPassword(accType, currUserID, pwd);
                write(nsd, &rslt, sizeof(rslt));
            }

            else if (select == 5)
            {
                if (option == 1)
                {
                    normalUser user1 = getNormalUser(currUserID);
                    write(nsd, &user1, sizeof(normalUser));
                }

                else if (option == 2)
                {
                    jointUser user2 = getJointUser(currUserID);
                    write(nsd, &user2, sizeof(jointUser));
                }
            }

            else if (select == 6)
            {
                break;
            }
        }

        else if (option == 3)
        {
            read(nsd, &type, sizeof(int));

            if (select == 1)
            {
                if (type == 1)
                {
                    normalUser newUser1;

                    read(nsd, &newUser1, sizeof(normalUser));
                    rslt = addNormalUser(newUser1);
                    write(nsd, &rslt, sizeof(rslt));
                }

                else if (type == 2)
                {
                    jointUser newUser2;

                    read(nsd, &newUser2, sizeof(jointUser));
                    rslt = addJointUser(newUser2);
                    write(nsd, &rslt, sizeof(rslt));
                }
            }

            else if (select == 2)
            {
                if (type == 1)
                {
                    int delUserID1;

                    read(nsd, &delUserID1, sizeof(int));
                    rslt = deleteNormalUser(delUserID1);
                    write(nsd, &rslt, sizeof(rslt));
                }

                else if (type == 2)
                {
                    int delUserID2;

                    read(nsd, &delUserID2, sizeof(int));
                    rslt = deleteJointUser(delUserID2);
                    write(nsd, &rslt, sizeof(rslt));
                }
            }

            else if (select == 3)
            {
                if (type == 1)
                {
                    normalUser modUser1;

                    read(nsd, &modUser1, sizeof(normalUser));
                    rslt = modifyNormalUser(modUser1);
                    write(nsd, &rslt, sizeof(rslt));
                }

                else if (type == 2)
                {
                    jointUser modUser2;

                    read(nsd, &modUser2, sizeof(jointUser));
                    rslt = modifyJointUser(modUser2);
                    write(nsd, &rslt, sizeof(rslt));
                }
            }

            else if (select == 4)
            {
                if (type == 1)
                {
                    normalUser searchUser1;
                    int userID1;

                    read(nsd, &userID1, sizeof(int));
                    searchUser1 = getNormalUser(userID1);
                    write(nsd, &searchUser1, sizeof(normalUser));
                }

                else if (type == 2)
                {
                    jointUser searchUser2;
                    int userID2;

                    read(nsd, &userID2, sizeof(int));
                    searchUser2 = getJointUser(userID2);
                    write(nsd, &searchUser2, sizeof(jointUser));
                }
            }

            else if (select == 5)
            {
                break;
            }
        }
    }
    
    close(nsd);
    write(1, "Ended client session.\n", sizeof("Ended client session.\n"));
    
    return;
}

void *connection_handler(void *nsd)
{
    int nsfd = *(int *)nsd;
    serverTask(nsfd);
}

int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;
    pthread_t threads;
    bool rslt;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5555);

    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 5); 

    write(1, "Waiting for the client.\n", sizeof("Waiting for the client.\n"));
    
    while (1)
    {
        clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

        write(1, "Connected to the client.\n", sizeof("Connected to the client.\n"));

        if (pthread_create(&threads, NULL, connection_handler, (void *)&nsd) < 0)
        {
            perror("could not create thread");
            return 1;
        }
    }

    pthread_exit(NULL);
    close(sd);
    
    return 0;
}