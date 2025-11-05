let age = 40;
age = 30;
console.log(age);

let totalScore = 100;
totalScore += 50;
console.log(totalScore);

var global1 = 100;
console.log(global1);
{
    console.log(global1);
}

let global2 = 90;
console.log(global2);
{
    console.log(global2);
}

{
    var local1 = 80;        
    console.log(local1);
}
console.log(local1);

{
    var local2 = 70;
    console.log(local2);
}
console.log(local2);


