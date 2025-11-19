let numbers = [1, [3,5], [8,[6,[9]]], 2];
let result = [];
function square(arr) {
    for(let x of arr) {
        if(typeof x == "number") {
            result.push(x*x); // If it's a number, insert its square into the array
        } else {
            square(x); // If it's not a number, execute recursively
        }
    }
}
square(numbers);
console.log(result);