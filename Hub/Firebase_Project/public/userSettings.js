var userListRef = database.ref('UsersList');
const usersTable = document.querySelector('.user-settings-table');
var count = 1;

userListRef.on("child_added", function(snap) {
    console.log("Added", snap.key, snap.val());
    addRowToTable(snap.key, snap.val());
});


function addRowToTable(key, user) {
    var row = usersTable.insertRow();
    var nameCell = row.insertCell(0);
    var usernameCell = row.insertCell(1);
    var emailCell = row.insertCell(2);
    var roleCell = row.insertCell(3);
    var deleteCell = row.insertCell(4);

    nameCell.innerHTML = user.fullname;
    usernameCell.innerHTML = user.username;
    emailCell.innerHTML = (user.email).slice(0,4)+"***"+user.email.slice((user.email).length-10);
    roleCell.innerHTML = user.role;
    deleteCell.innerHTML = `<button onclick="removeAccount('${user.username}')">[delete]</button>`;
    count += 1;
};

function removeAccount(accountName) {
    userListRef.child(accountName).remove();
    for (let i = 0; i < usersTable.rows.length; i++) {
        var username = usersTable.rows[i].cells[1].innerText
        if (username == accountName) {
            usersTable.deleteRow(i);
        }
      }
}