<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Kotakamalpintar</title>
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;600&display=swap');
        * {
            font-family: 'Poppins', sans-serif;
        }
        table {
            width: 100%;
            border-collapse: collapse;
        }
        table, th, td {
            border: 1px solid #ddd;
        }
        th, td {
            padding: 10px;
            text-align: left;
        }
        th {
            background-color: #f4f4f4;
        }
    </style>
</head>
<body class="bg-white">

<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "kotak_amal_pintar";

// Membuat koneksi
$conn = new mysqli($servername, $username, $password, $dbname);

// Memeriksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}
?>

<header class="flex items-center justify-between w-full h-16 bg-white border-b-4 border-blue-700 px-6">
    <img class="w-8 h-8" src="polindra.png" alt="Logo">
    <div class="text-sm font-semibold text-blue-700">KOTAK AMAL PINTAR</div>
    <div class="text-xs py-1 px-3 bg-blue-300 text-white rounded-full">admin</div>
</header>

<div class="w-4/5 m-auto mt-10">

    <p class="text-lg font-bold mb-5">Tabel Data Sedekah</p>

    <div class="w-full h-auto rounded-lg border-3 border-b-7 border-blue-700 p-6">
        <table class="w-full">
            <thead>
                <tr>
                    <th class="text-sm font-semibold">ID</th>
                    <th class="text-sm font-semibold">Nominal (Rp)</th>
                    <th class="text-sm font-semibold">Timestamp</th>
                </tr>
            </thead>
            <tbody>
                <?php
                // Fetch all data from the 'sedekah' table
                $sql = "SELECT id, nominal, timestamp FROM sedekah ORDER BY id DESC";
                $result = $conn->query($sql);

                if ($result->num_rows > 0) {
                    while ($row = $result->fetch_assoc()) {
                        $formattedNominal = number_format($row['nominal'], 0, ',', '.');
                        $formattedTimestamp = date("d-m-Y H:i:s", strtotime($row['timestamp']));
                        echo "<tr>
                                <td class='text-sm'>{$row['id']}</td>
                                <td class='text-sm'>{$formattedNominal}</td>
                                <td class='text-sm'>{$formattedTimestamp}</td>
                              </tr>";
                    }
                } else {
                    echo "<tr><td colspan='3' class='text-sm text-center'>No data found</td></tr>";
                }
                ?>
            </tbody>
        </table>
    </div>

</div>

</body>
</html>
