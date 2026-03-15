import os
import glob
import pandas as pd
import mplfinance as mpf
import matplotlib
matplotlib.use('Agg')

LOGS_DIR = 'logs'
CHARTS_DIR = 'charts'

os.makedirs(CHARTS_DIR, exist_ok=True)

for filepath in glob.glob(os.path.join(LOGS_DIR, '*.txt')):
    try:
        if os.path.getsize(filepath) == 0:
            continue

        # load data
        df = pd.read_csv(filepath, sep=' ', names=['timestamp', 'price', 'qty'])
        if df.empty:
            continue

        df.index = pd.to_datetime(df.index, unit='s')
        
        # convert to candlesticks
        ohlc = df['price'].resample('100s').ohlc()
        ohlc['volume'] = df['qty'].resample('100s').sum()
        ohlc = ohlc.dropna()

        filename = os.path.basename(filepath).replace('.txt', '.png')
        save_path = os.path.join(CHARTS_DIR, filename)
        
        mpf.plot(ohlc, type='candle', volume=True, style='charles', savefig=save_path)
        print(f"Generated chart: {save_path}")
        
    except Exception as e:
        print(f"Error processing {filepath}: {e}")