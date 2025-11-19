// if
let value = 2;

if (value > 3) {
    value += 1;
} else if (value <= 2) {
    value -= 2;
} else {
    value += 2;
}

console.log(value);

// for
for (let i = 0; i < 5; i++) {
    if (i < 2) {
        continue;
    }

    console.log(i);

    if (i == 3) {
        break;
    }
}

// function
console.log(isEven(4));
console.log(isEven(7));

function isEven(num) {
    if (num % 2 == 0) {
        return true;
    } else {
        return false;
    }
}