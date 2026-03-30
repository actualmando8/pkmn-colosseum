    nofralloc
    cmplwi r3, 0x0
    lis r4, lbl_803FC620@ha
    addi r4, r4, lbl_803FC620@l
    beq @L_800B018C
    mr r0, r3
    b @L_800B0190
    @L_800B018C:
    addi r0, r4, 0x220
    @L_800B0190:
    cmplwi r3, 0x0
    stw r0, 0x10c(r4)
    beq @L_800B01A0
    b @L_800B01A4
    @L_800B01A0:
    addi r3, r4, 0x220
    @L_800B01A4:
    stw r3, 0x21c(r4)
    blr
