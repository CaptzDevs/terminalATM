   printf("New First Name : ");
                    scanf("%[^\n]%*c", nFname);
                    printf("New Last Name : ");
                    scanf("%[^\n]%*c", nLname);
                    printf("New Age :");
                    scanf("%d",&nAge);

                    printf("%s \n",userDetail->data.id);
                    printf("%s -> %s \n",userDetail->data.fname,nFname);
                    printf("%s -> %s \n",userDetail->data.lname,nLname);
                    printf("%d -> %d \n",userDetail->data.age,nAge);