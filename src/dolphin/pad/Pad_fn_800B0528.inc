    nofralloc
    li r0, 0x8
    mtctr r0
    srwi r6, r3, 31
    li r9, 0x0
    li r10, 0x1
    li r7, 0x0
    li r8, 0x0
    li r5, 0x1
    @L_800B0548:
    cmplwi r8, 0xf
    ble @L_800B0578
    cmplwi r8, 0x1f
    bne @L_800B0560
    or r7, r7, r6
    b @L_800B0594
    @L_800B0560:
    slw r0, r5, r8
    and r0, r3, r0
    srw r0, r0, r10
    or r7, r7, r0
    addi r10, r10, 0x2
    b @L_800B0594
    @L_800B0578:
    slw r4, r5, r8
    subfic r0, r8, 0x1f
    subf r0, r9, r0
    and r4, r3, r4
    slw r0, r4, r0
    or r7, r7, r0
    addi r9, r9, 0x1
    @L_800B0594:
    addi r8, r8, 0x1
    cmplwi r8, 0xf
    ble @L_800B05C8
    cmplwi r8, 0x1f
    bne @L_800B05B0
    or r7, r7, r6
    b @L_800B05E4
    @L_800B05B0:
    slw r0, r5, r8
    and r0, r3, r0
    srw r0, r0, r10
    or r7, r7, r0
    addi r10, r10, 0x2
    b @L_800B05E4
    @L_800B05C8:
    slw r4, r5, r8
    subfic r0, r8, 0x1f
    subf r0, r9, r0
    and r4, r3, r4
    slw r0, r4, r0
    or r7, r7, r0
    addi r9, r9, 0x1
    @L_800B05E4:
    addi r8, r8, 0x1
    cmplwi r8, 0xf
    ble @L_800B0618
    cmplwi r8, 0x1f
    bne @L_800B0600
    or r7, r7, r6
    b @L_800B0634
    @L_800B0600:
    slw r0, r5, r8
    and r0, r3, r0
    srw r0, r0, r10
    or r7, r7, r0
    addi r10, r10, 0x2
    b @L_800B0634
    @L_800B0618:
    slw r4, r5, r8
    subfic r0, r8, 0x1f
    subf r0, r9, r0
    and r4, r3, r4
    slw r0, r4, r0
    or r7, r7, r0
    addi r9, r9, 0x1
    @L_800B0634:
    addi r8, r8, 0x1
    cmplwi r8, 0xf
    ble @L_800B0668
    cmplwi r8, 0x1f
    bne @L_800B0650
    or r7, r7, r6
    b @L_800B0684
    @L_800B0650:
    slw r0, r5, r8
    and r0, r3, r0
    srw r0, r0, r10
    or r7, r7, r0
    addi r10, r10, 0x2
    b @L_800B0684
    @L_800B0668:
    slw r4, r5, r8
    subfic r0, r8, 0x1f
    subf r0, r9, r0
    and r4, r3, r4
    slw r0, r4, r0
    or r7, r7, r0
    addi r9, r9, 0x1
    @L_800B0684:
    addi r8, r8, 0x1
    bdnz @L_800B0548
    mr r3, r7
    blr
