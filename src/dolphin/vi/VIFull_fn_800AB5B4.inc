    nofralloc
    li r0, 0x0
    cmpwi r3, 0x1
    stw r0, __PADSpec(r13)
    beq @L_800AB5F0
    bge @L_800AB5D4
    cmpwi r3, 0x0
    bge @L_800AB5E0
    b @L_800AB60C
    @L_800AB5D4:
    cmpwi r3, 0x6
    bge @L_800AB60C
    b @L_800AB600
    @L_800AB5E0:
    lis r4, fn_800AB614@ha
    addi r0, r4, fn_800AB614@l
    stw r0, lbl_80478A1C(r13)
    b @L_800AB60C
    @L_800AB5F0:
    lis r4, fn_800AB788@ha
    addi r0, r4, fn_800AB788@l
    stw r0, lbl_80478A1C(r13)
    b @L_800AB60C
    @L_800AB600:
    lis r4, fn_800AB8FC@ha
    addi r0, r4, fn_800AB8FC@l
    stw r0, lbl_80478A1C(r13)
    @L_800AB60C:
    stw r3, lbl_80478A18(r13)
    blr
