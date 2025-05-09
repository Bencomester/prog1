<?php

class Csomopont {
    private string $betu;
    private ?Csomopont $balNulla = null;
    private ?Csomopont $jobbEgy = null;

    public function __construct(string $b = '/') {
        $this->betu = $b;
    }

    public function nullasGyermek(): ?Csomopont {
        return $this->balNulla;
    }

    public function egyesGyermek(): ?Csomopont {
        return $this->jobbEgy;
    }

    public function ujNullasGyermek(Csomopont $gy): void {
        $this->balNulla = $gy;
    }

    public function ujEgyesGyermek(Csomopont $gy): void {
        $this->jobbEgy = $gy;
    }

    public function getBetu(): string {
        return $this->betu;
    }
}

class LZWBinFa {
    private Csomopont $gyoker;
    private Csomopont $fa;
    private int $melyseg = 0;
    private int $maxMelyseg = 0;
    private int $atlagosszeg = 0;
    private int $atlagdb = 0;
    private float $atlag = 0.0;
    private float $szorasosszeg = 0.0;
    private float $szoras = 0.0;

    public function __construct() {
        $this->gyoker = new Csomopont();
        $this->fa = $this->gyoker;
    }

    // Removed the destructor as PHP's garbage collection will handle it
    // and recursive destruction might cause issues

    public function addChar(string $b): void {
        if ($b === '0') {
            if (!$this->fa->nullasGyermek()) {
                $uj = new Csomopont('0');
                $this->fa->ujNullasGyermek($uj);
                $this->fa = $this->gyoker;
            } else {
                $this->fa = $this->fa->nullasGyermek();
            }
        } else {
            if (!$this->fa->egyesGyermek()) {
                $uj = new Csomopont('1');
                $this->fa->ujEgyesGyermek($uj);
                $this->fa = $this->gyoker;
            } else {
                $this->fa = $this->fa->egyesGyermek();
            }
        }
    }

    public function kiir(): void {
        $this->melyseg = 0;
        $this->kiirRekurziv($this->gyoker, STDOUT);
    }

    private function kiirRekurziv(?Csomopont $elem, $out): void {
        if ($elem !== null) {
            $this->melyseg++;
            $this->kiirRekurziv($elem->egyesGyermek(), $out);
            for ($i = 0; $i < $this->melyseg; ++$i) {
                fwrite($out, "---");
            }
            fwrite($out, $elem->getBetu() . "(" . ($this->melyseg - 1) . ")\n");
            $this->kiirRekurziv($elem->nullasGyermek(), $out);
            $this->melyseg--;
        }
    }

    public function getMelyseg(): int {
        $this->melyseg = 0;
        $this->maxMelyseg = 0;
        $this->rmelyseg($this->gyoker);
        return $this->maxMelyseg - 1;
    }

    private function rmelyseg(?Csomopont $elem): void {
        if ($elem !== null) {
            $this->melyseg++;
            if ($this->melyseg > $this->maxMelyseg)
                $this->maxMelyseg = $this->melyseg;
            $this->rmelyseg($elem->egyesGyermek());
            $this->rmelyseg($elem->nullasGyermek());
            $this->melyseg--;
        }
    }

    public function getAtlag(): float {
        $this->melyseg = 0;
        $this->atlagosszeg = 0;
        $this->atlagdb = 0;
        $this->ratlag($this->gyoker);
        if ($this->atlagdb > 0) { // Avoid division by zero
            $this->atlag = $this->atlagosszeg / $this->atlagdb;
        } else {
            $this->atlag = 0.0;
        }
        return $this->atlag;
    }

    private function ratlag(?Csomopont $elem): void {
        if ($elem !== null) {
            $this->melyseg++;
            $this->ratlag($elem->egyesGyermek());
            $this->ratlag($elem->nullasGyermek());
            $this->melyseg--;
            if ($elem->egyesGyermek() === null && $elem->nullasGyermek() === null) {
                $this->atlagdb++;
                $this->atlagosszeg += $this->melyseg;
            }
        }
    }

    public function getSzoras(): float {
        $this->atlag = $this->getAtlag();
        $this->szorasosszeg = 0.0;
        $this->melyseg = 0;
        $this->atlagdb = 0;
        $this->rszoras($this->gyoker);

        if ($this->atlagdb - 1 > 0) {
            $this->szoras = sqrt($this->szorasosszeg / ($this->atlagdb - 1));
        } else {
            $this->szoras = sqrt($this->szorasosszeg);
        }
        return $this->szoras;
    }

    private function rszoras(?Csomopont $elem): void {
        if ($elem !== null) {
            $this->melyseg++;
            $this->rszoras($elem->egyesGyermek());
            $this->rszoras($elem->nullasGyermek());
            $this->melyseg--;
            if ($elem->egyesGyermek() === null && $elem->nullasGyermek() === null) {
                $this->atlagdb++;
                $this->szorasosszeg += ($this->melyseg - $this->atlag) ** 2;
            }
        }
    }

    public function __toString(): string {
        ob_start();
        $this->kiirRekurziv($this->gyoker, fopen('php://output', 'wb'));
        return ob_get_clean();
    }
}

function usage(): void {
    echo "Usage: php lzwtree.php in_file -o out_file\n";
}

function main(array $argv): void {
    if (count($argv) !== 4) {
        usage();
        exit(1);
    }

    $inFile = $argv[1];
    if ($argv[2] !== "-o") {
        usage();
        exit(2);
    }

    $outFile = $argv[3];

    if (!file_exists($inFile)) {
        echo "{$inFile} does not exist...\n";
        usage();
        exit(3);
    }

    $input = fopen($inFile, "rb");
    if (!$input) {
        echo "Failed to open input file {$inFile}\n";
        exit(4);
    }

    $output = fopen($outFile, 'wb');
    if (!$output) {
        echo "Failed to open output file {$outFile}\n";
        fclose($input);
        exit(5);
    }

    $binFa = new LZWBinFa();

    // Skip the first line in a safer way
    $line = fgets($input);
    if ($line === false) {
        echo "Input file is empty\n";
        fclose($input);
        fclose($output);
        exit(6);
    }

    $kommentben = false;
    $count = 0; // Track processed bytes to detect potential infinite loops
    $lastPos = ftell($input);

    while (!feof($input) && ($b = fread($input, 1)) !== false) {
        $byte = ord($b);

        // Safety check to avoid infinite loops
        $currentPos = ftell($input);
        if ($currentPos == $lastPos) {
            $count++;
            if ($count > 100) { // Prevent infinite loops
                echo "Possible infinite loop detected\n";
                break;
            }
        } else {
            $lastPos = $currentPos;
            $count = 0;
        }

        if ($byte === 0x3e) {
            $kommentben = true;
            continue;
        }

        if ($byte === 0x0a) {
            $kommentben = false;
            continue;
        }

        if ($kommentben || $byte === 0x4e) {
            continue;
        }

        for ($i = 0; $i < 8; ++$i) {
            $binFa->addChar(($byte & 0x80) ? '1' : '0');
            $byte <<= 1;
        }
    }

    fwrite($output, (string)$binFa);
    fwrite($output, "depth = " . $binFa->getMelyseg() . "\n");
    fwrite($output, "mean = " . $binFa->getAtlag() . "\n");
    fwrite($output, "var = " . $binFa->getSzoras() . "\n");

    fclose($input);
    fclose($output);

    echo "Processing complete, output written to {$outFile}\n";
}

main($argv);
