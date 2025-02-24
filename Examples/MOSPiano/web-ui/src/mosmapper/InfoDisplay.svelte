<script lang="ts">
    import type { nodecoord } from './lattice_math';
    import {calc_note_label} from './lattice_math';
    export let s_a;
    export let s_b;
    export let mode;
    export let scale_base:nodecoord[];
    export let tuning_base_freq;
    export let tuning;
    export let playing_nodecoords:nodecoord[];

    $:a_freq = tuning.coord_to_freq(1, 0);
    $:b_freq = tuning.coord_to_freq(0, 1);
    $:a_large = a_freq > b_freq;
    $:L = a_large ? s_a : s_b;
    $:s = a_large ? s_b : s_a;
    $:octave_freq = tuning.coord_to_freq(s_a, s_b);
    $:octave_ct = 1200. * Math.log(octave_freq)/Math.log(2);
    $:L_freq = a_large?a_freq:b_freq;
    $:L_ct = 1200. * Math.log(L_freq)/Math.log(2);
    $:s_freq = a_large?b_freq:a_freq;
    $:s_ct = 1200. * Math.log(s_freq)/Math.log(2);
    $:L_to_s = L_ct/s_ct;

    $:sequence = [...Array(scale_base.length-1).keys()].map((_,i) => (scale_base[i+1].aa===scale_base[i].aa) !== a_large?'L':'s').join('');

    $: note_labels = playing_nodecoords.map((n) => calc_note_label({a:s_a, b:s_b}, n, a_large, L==5 && s==2));

    // Equalized (L:s = 1:1)
    // Ultrasoft	1:1 ≤ L:s ≤ 4:3	1.000 ≤ L/s ≤ 1.333
    // Supersoft (L:s = 4:3)	
    // Parasoft	4:3 ≤ L:s ≤ 3:2	1.333 ≤ L/s ≤ 1.500
    // Soft (L:s = 3:2)	
    // Quasisoft	3:2 ≤ L:s ≤ 5:3	1.500 ≤ L/s ≤ 1.667
    // Semisoft (L:s = 5:3)	
    // Minisoft	5:3 ≤ L:s ≤ 2:1	1.667 ≤ L/s ≤ 2.000
    // Basic (L:s = 2:1)
    // Minihard	2:1 ≤ L:s ≤ 5:2	2.000 ≤ L/s ≤ 2.500
    // Semihard (L:s = 5:2)	
    // Quasihard	5:2 ≤ L:s ≤ 3:1	2.500 ≤ L/s ≤ 3.000
    // Hard (L:s = 3:1)	
    // Parahard	3:1 ≤ L:s ≤ 4:1	3.000 ≤ L/s ≤ 4.000
    // Superhard (L:s = 4:1)	
    // Ultrahard	4:1 ≤ L:s ≤ 1:0	4.000 ≤ L/s ≤ ∞
    // Collapsed (L:s = 1:0)


    
    const epsilon = 3e-2;

    let calc_L_to_s_label = (ltos:number) => {
        if (ltos < 1+epsilon) return 'Equalized';
        if (ltos < 4/3-epsilon) return 'Ultrasoft';
        if (ltos < 4/3+epsilon) return 'Supersoft';
        if (ltos < 3/2-epsilon) return 'Parasoft';
        if (ltos < 3/2+epsilon) return 'Soft';
        if (ltos < 5/3-epsilon) return 'Quasisoft';
        if (ltos < 5/3+epsilon) return 'Semisoft';
        if (ltos < 2-epsilon) return 'Minisoft';
        if (ltos < 2+epsilon) return 'Basic';
        if (ltos < 5/2-epsilon) return 'Minihard';
        if (ltos < 5/2+epsilon) return 'Semihard';
        if (ltos < 3-epsilon) return 'Quasihard';
        if (ltos < 3+epsilon) return 'Hard';
        if (ltos < 4-epsilon) return 'Parahard';
        if (ltos < 4+epsilon) return 'Superhard';
        if (ltos < 1/(1+epsilon)) return 'Ultrahard';
        return 'Collapsed';
    }
    $: L_to_s_label = calc_L_to_s_label(L_to_s);


</script>

<style>
    .container {
        position:absolute;
        width:320px;
        height:220px;
        right:0px;
        top:0;
        background-color: #444;
        color: #ddd;
        text-align: left;
        font-size: 2;
    }

</style>

<div class="container">
    <table  style="width:100%"><tbody>
        <tr>
            <td style="width:33%">System</td>
            <td style="width:33%">L{L}s{s} m{mode}</td>
            <td style="width:33%">{sequence}</td>
        </tr>
        <tr><td>(L)arge</td><td>{L_ct.toFixed(1)}ct</td></tr>
        <tr><td>(s)mall</td><td>{s_ct.toFixed(1)}ct</td></tr>
        <tr><td>Hardness</td><td>L/s={(L_ct/s_ct).toFixed(2)}</td><td>{L_to_s_label}</td></tr>
        <tr><td>Base</td><td>{tuning_base_freq.toFixed(2)}Hz</td></tr>
        <tr><td>Equave</td><td>{(tuning_base_freq*octave_freq).toFixed(2)}Hz</td><td>{octave_ct.toFixed(1)}ct</td></tr>
        <tr><td>Notes</td><td>{note_labels.join(' ')}</td><td></td></tr>
    </tbody></table>
</div>
