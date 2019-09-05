## How to modify the structure of a multi index table

Modifying a multi index table structure that has already been deployed to an EOSIO based blockchain has to be done depending on your requirements by following different strategies which we are outlining below:

### If you don't mind to lose the existing data

If you don't mind to lose the data from the initial table you can follow these two steps
- Erase all records from first table
- Deploy a new contract with modified table structure

### If you want to keep the existing data

If you want to keep the existing data there are two ways to do it:

#### 1. Using binary extentions
To learn how to modify the structure using binary extensions please read this [tutorial](../../09_tutorials/binary-extension.md)

#### 2. Migrate the existing data to a second table

##### A. Migration without downtime, but slower

1. Create the new version of your multi index table alongside the old one,
2. Transfer data from the old table to the new one. You may do so as part of your normal access pattern, first checking the new table to see if the entry you seek is present and if not, check the original table, and if it's present, migrate it while adding the data for the new field, then remove it from the original table to save RAM costs, 
3. You must retain both versions of your multi index table until you have completed this migration, at which point you may update your contract to remove the original version of your multi index table.

##### B. Migration with downtime, but faster

If you prefer less code complexity and can accept downtime for your application:
1. Deploy a version of your contract solely for migration purposes, and run migration transactions on every row of your table until complete. If the first table is big, e.g. has a big number of rows, the transaction time limit could be reached while running the  migration transactions. To mitigate this implement the migrate function to move a reduced number of rows each time it runs,
2. Deploy a new contract using only the new version of the table, at which point, your migration and downtime is complete.

Both of the above migration methods require some pre-planning (like the ability to put your contract into a maintenance mode for user feedback)