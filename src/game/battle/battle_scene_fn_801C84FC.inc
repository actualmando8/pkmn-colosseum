    nofralloc
    lfs f1, 0x14(r3)
    lfs f0, 0x14(r4)
    fcmpu cr0, f1, f0
    bne @L_801C8514
    li r3, 0x0
    blr
@L_801C8514:
    fcmpo cr0, f1, f0
    ble @L_801C8524
    li r3, 0x1
    blr
@L_801C8524:
    li r3, -0x1
    blr
